#ifdef PCP_HAS_XMPP

extern "C" {
#include <strophe.h>
}

#include "pcp/xmpp_client.hpp"
#include "pcp/exporter.hpp"
#include <android/log.h>
#include <sstream>
#include <cstring>
#include <chrono>

#define LOG_TAG "PcpXmpp"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {

static void xmppLogHandler(void* /*userdata*/,
                            const xmpp_log_level_t /*level*/,
                            const char* const /*area*/,
                            const char* const msg) {
    if (msg) {
        LOGD("libstrophe: %s", msg);
    }
}

static std::string jidBare(const std::string& fullJid) {
    auto slash = fullJid.find('/');
    if (slash != std::string::npos) {
        return fullJid.substr(0, slash);
    }
    return fullJid;
}

// ====== XmppClient ======

XmppClient::XmppClient() {
    xmpp_initialize();
    xmpp_log_t* log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG);
    ctx = xmpp_ctx_new(nullptr, log);
    if (!ctx) {
        LOGE("Failed to create XMPP context");
    }
}

XmppClient::~XmppClient() {
    disconnect();
    if (ctx) {
        xmpp_ctx_free(ctx);
        ctx = nullptr;
    }
    xmpp_shutdown();
}

bool XmppClient::connect(const XmppAccount& acc) {
    if (running) {
        LOGE("Already connected/running");
        return false;
    }
    if (!ctx) {
        LOGE("No XMPP context");
        return false;
    }

    account = acc;
    ownJid = account.jid;

    conn = xmpp_conn_new(ctx);
    if (!conn) {
        LOGE("Failed to create XMPP connection");
        return false;
    }

    xmpp_conn_set_jid(conn, account.jid.c_str());
    xmpp_conn_set_pass(conn, account.password.c_str());

    std::string server = account.server;
    if (server.empty()) {
        auto at = account.jid.find('@');
        if (at != std::string::npos) {
            server = account.jid.substr(at + 1);
        }
    }

    int port = account.port > 0 ? account.port : 5222;

    authenticated = false;

    int result = xmpp_connect_client(
        conn,
        server.empty() ? nullptr : server.c_str(),
        port,
        XmppClient::s_onConnect,
        this
    );

    if (result != 0) {
        LOGE("xmpp_connect_client returned %d", result);
        xmpp_conn_release(conn);
        conn = nullptr;
        return false;
    }

    xmpp_handler_add(conn, XmppClient::s_onMessage, nullptr, "message", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onPresence, nullptr, "presence", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onMamIq, "urn:xmpp:mam:2", "message", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onMamIq, "urn:xmpp:mam:2", "iq", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onVCardIq, "vcard-temp", "iq", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onUploadIq, "urn:xmpp:http:upload:0", "iq", nullptr, this);
    xmpp_handler_add(conn, XmppClient::s_onMessage, "urn:xmpp:carbons:2", "message", nullptr, this);

    running = true;
    eventThread = std::thread(&XmppClient::eventLoop, this);

    return true;
}

void XmppClient::disconnect() {
    running = false;

    if (eventThread.joinable()) {
        eventThread.join();
    }

    if (conn) {
        xmpp_disconnect(conn);
        xmpp_conn_release(conn);
        conn = nullptr;
    }

    authenticated = false;

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            eventCallback(XmppEventType::CONNECTION_CHANGED, R"({"state":"DISCONNECTED"})");
        }
    }
}

bool XmppClient::isConnected() const {
    return running && conn != nullptr;
}

bool XmppClient::isAuthenticated() const {
    return authenticated;
}

std::string XmppClient::getOwnJid() const {
    return ownJid;
}

void XmppClient::sendMessage(const std::string& toJid, const std::string& body) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* msg = xmpp_stanza_new(ctx);
    if (!msg) return;

    xmpp_stanza_set_name(msg, "message");
    xmpp_stanza_set_type(msg, "chat");
    xmpp_stanza_set_attribute(msg, "to", toJid.c_str());

    xmpp_stanza_t* bodyEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(bodyEl, "body");
    xmpp_stanza_set_text(bodyEl, body.c_str());
    xmpp_stanza_add_child(msg, bodyEl);
    xmpp_stanza_release(bodyEl);

    xmpp_send(conn, msg);
    xmpp_stanza_release(msg);

    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            std::ostringstream json;
            json << R"({"chatJid":")" << escapeJson(toJid)
                 << R"(","senderJid":")" << escapeJson(ownJid)
                 << R"(","senderName":")" << escapeJson(ownJid)
                 << R"(","body":")" << escapeJson(body) << R"(")"
                 << R"(,"timestamp":)" << ts
                 << R"(,"isOutgoing":true,"type":"chat"})";
            eventCallback(XmppEventType::MESSAGE_RECEIVED, json.str());
        }
    }
}

void XmppClient::sendPresence(const std::string& status, const std::string& show) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");

    if (!show.empty()) {
        xmpp_stanza_t* showEl = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(showEl, "show");
        xmpp_stanza_set_text(showEl, show.c_str());
        xmpp_stanza_add_child(pres, showEl);
        xmpp_stanza_release(showEl);
    }

    if (!status.empty()) {
        xmpp_stanza_t* statusEl = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(statusEl, "status");
        xmpp_stanza_set_text(statusEl, status.c_str());
        xmpp_stanza_add_child(pres, statusEl);
        xmpp_stanza_release(statusEl);
    }

    xmpp_send(conn, pres);
    xmpp_stanza_release(pres);
}

void XmppClient::sendTypingNotification(const std::string& toJid, bool isTyping) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* msg = xmpp_stanza_new(ctx);
    if (!msg) return;

    xmpp_stanza_set_name(msg, "message");
    xmpp_stanza_set_type(msg, "chat");
    xmpp_stanza_set_attribute(msg, "to", toJid.c_str());

    const char* childName = isTyping ? "composing" : "active";
    xmpp_stanza_t* state = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(state, childName);
    xmpp_stanza_set_ns(state, "http://jabber.org/protocol/chatstates");
    xmpp_stanza_add_child(msg, state);
    xmpp_stanza_release(state);

    xmpp_send(conn, msg);
    xmpp_stanza_release(msg);
}

void XmppClient::joinRoom(const std::string& roomJid) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");

    std::string toAddr = roomJid + "/" + account.resource;
    xmpp_stanza_set_attribute(pres, "to", toAddr.c_str());

    xmpp_stanza_t* x = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(x, "x");
    xmpp_stanza_set_ns(x, "http://jabber.org/protocol/muc");
    xmpp_stanza_add_child(pres, x);
    xmpp_stanza_release(x);

    xmpp_send(conn, pres);
    xmpp_stanza_release(pres);
}

void XmppClient::leaveRoom(const std::string& roomJid) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");
    xmpp_stanza_set_type(pres, "unavailable");

    std::string toAddr = roomJid + "/" + account.resource;
    xmpp_stanza_set_attribute(pres, "to", toAddr.c_str());

    xmpp_send(conn, pres);
    xmpp_stanza_release(pres);
}

void XmppClient::setNickname(const std::string& roomJid, const std::string& nick) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");

    std::string toAddr = roomJid + "/" + nick;
    xmpp_stanza_set_attribute(pres, "to", toAddr.c_str());

    xmpp_send(conn, pres);
    xmpp_stanza_release(pres);
}

std::vector<XmppUser> XmppClient::getRoster() const {
    std::lock_guard<std::mutex> lock(rosterMutex);
    return roster;
}

std::vector<XmppChat> XmppClient::getChats() const {
    std::lock_guard<std::mutex> lock(chatsMutex);
    return chats;
}

void XmppClient::setEventCallback(EventCallback cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    eventCallback = std::move(cb);
}

void XmppClient::runOnce(int timeoutMs) {
    if (!ctx || !running) return;
    xmpp_run_once(ctx, timeoutMs);
}

void XmppClient::eventLoop() {
    while (running) {
        if (ctx) {
            xmpp_run_once(ctx, 100);
        }
    }
}

void XmppClient::onConnect(xmpp_conn_t* /*conn*/, int status) {
    if (status == 0) {
        authenticated = true;
        {
            std::lock_guard<std::mutex> lock(callbackMutex);
            if (eventCallback) {
                eventCallback(XmppEventType::CONNECTION_CHANGED, R"({"state":"CONNECTED"})");
                eventCallback(XmppEventType::AUTH_CHANGED, R"({"authenticated":true})");
            }
        }
    } else {
        authenticated = false;
        {
            std::lock_guard<std::mutex> lock(callbackMutex);
            if (eventCallback) {
                eventCallback(XmppEventType::CONNECTION_CHANGED, R"({"state":"ERROR"})");
                eventCallback(XmppEventType::ERROR,
                    R"({"message":"XMPP connection failed"})");
            }
        }
    }
}

void XmppClient::onMessage(const std::string& from, const std::string& body,
                            const std::string& type, bool isMuc) {
    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();

    std::string chatJid = from;
    std::string senderJid = from;
    std::string senderName = from;

    if (isMuc) {
        chatJid = jidBare(from);
        auto slash = from.find('/');
        if (slash != std::string::npos) {
            senderName = from.substr(slash + 1);
        }
    }

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            std::ostringstream json;
            json << R"({"chatJid":")" << escapeJson(chatJid)
                 << R"(","senderJid":")" << escapeJson(senderJid)
                 << R"(","senderName":")" << escapeJson(senderName)
                 << R"(","body":")" << escapeJson(body) << R"(")"
                 << R"(,"timestamp":)" << ts
                 << R"(,"isOutgoing":false,"type":")" << escapeJson(type) << R"("})";
            eventCallback(XmppEventType::MESSAGE_RECEIVED, json.str());
        }
    }
}

void XmppClient::onPresence(const std::string& from, const std::string& status,
                             const std::string& show) {
    {
        std::lock_guard<std::mutex> lock(rosterMutex);
        bool found = false;
        for (auto& u : roster) {
            if (u.jid == jidBare(from)) {
                u.status = status;
                u.isOnline = (show != "unavailable");
                found = true;
                break;
            }
        }
        if (!found) {
            XmppUser user;
            user.jid = jidBare(from);
            user.nickname = jidBare(from);
            user.status = status;
            user.isOnline = (show != "unavailable");
            roster.push_back(std::move(user));
        }
    }

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            std::ostringstream json;
            json << R"({"jid":")" << escapeJson(from)
                 << R"(","status":")" << escapeJson(status)
                 << R"(","show":")" << escapeJson(show) << R"("})";
            eventCallback(XmppEventType::PRESENCE_CHANGED, json.str());
        }
    }
}

// ====== Static callbacks ======

void XmppClient::s_onConnect(xmpp_conn_t* conn, xmpp_conn_event_t status, int error,
                              xmpp_stream_error_t* /*streamError*/, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);
    (void)error;
    client->onConnect(conn, static_cast<int>(status));
}

int XmppClient::s_onMessage(xmpp_conn_t* /*conn*/, xmpp_stanza_t* stanza, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);

    const char* from = xmpp_stanza_get_attribute(stanza, "from");
    const char* type = xmpp_stanza_get_type(stanza);
    bool isMuc = type && strcmp(type, "groupchat") == 0;

    xmpp_stanza_t* carbonEnv = xmpp_stanza_get_child_by_ns(stanza, "urn:xmpp:carbons:2");
    if (carbonEnv) {
        const char* envName = xmpp_stanza_get_name(carbonEnv);
        bool isReceived = envName && strcmp(envName, "received") == 0;
        xmpp_stanza_t* forwarded = xmpp_stanza_get_child_by_ns(carbonEnv, "urn:xmpp:forward:0");
        if (!forwarded) {
            forwarded = xmpp_stanza_get_child_by_name(carbonEnv, "forwarded");
        }
        if (forwarded) {
            xmpp_stanza_t* innerMsg = xmpp_stanza_get_child_by_name(forwarded, "message");
            if (innerMsg) {
                const char* innerFrom = xmpp_stanza_get_attribute(innerMsg, "from");
                const char* innerType = xmpp_stanza_get_type(innerMsg);
                xmpp_stanza_t* innerBody = xmpp_stanza_get_child_by_name(innerMsg, "body");
                const char* innerBodyText = nullptr;
                if (innerBody) innerBodyText = xmpp_stanza_get_text(innerBody);
                if (innerBodyText && innerFrom) {
                    client->onMessage(
                        isReceived ? innerFrom : from,
                        innerBodyText,
                        "chat",
                        false
                    );
                }
            }
        }
        return 1;
    }

    xmpp_stanza_t* bodyEl = xmpp_stanza_get_child_by_name(stanza, "body");
    const char* bodyText = nullptr;
    if (bodyEl) {
        bodyText = xmpp_stanza_get_text(bodyEl);
    }

    if (isMuc) {
        xmpp_stanza_t* subjectEl = xmpp_stanza_get_child_by_name(stanza, "subject");
        if (subjectEl) {
            const char* subjectText = xmpp_stanza_get_text(subjectEl);
            if (subjectText && from) {
                std::string roomJid = jidBare(from);
                std::string nick;
                auto slash = std::string(from).find('/');
                if (slash != std::string::npos) {
                    nick = std::string(from).substr(slash + 1);
                }
                client->onMucSubject(roomJid, subjectText, nick);
            }
        }
    }

    if (bodyText && from) {
        client->onMessage(
            from,
            bodyText,
            type ? type : "chat",
            isMuc
        );
    }

    xmpp_stanza_t* xMuc = xmpp_stanza_get_child_by_ns(stanza, "http://jabber.org/protocol/muc#user");
    if (xMuc && from) {
        std::string roomJid = jidBare(from);
        xmpp_stanza_t* invite = xmpp_stanza_get_child_by_name(xMuc, "invite");
        if (invite) {
            const char* inviter = xmpp_stanza_get_attribute(invite, "from");
            if (inviter) {
                std::lock_guard<std::mutex> lock(client->callbackMutex);
                if (client->eventCallback) {
                    std::ostringstream json;
                    json << R"({"roomJid":")" << escapeJson(roomJid)
                         << R"(","inviter":")" << escapeJson(inviter)
                         << R"(","type":"invitation"})";
                    client->eventCallback(XmppEventType::ERROR, json.str());
                }
            }
        }
    }

    return 1;
}

int XmppClient::s_onPresence(xmpp_conn_t* /*conn*/, xmpp_stanza_t* stanza, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);

    const char* from = xmpp_stanza_get_attribute(stanza, "from");
    const char* type = xmpp_stanza_get_type(stanza);

    std::string show = "available";
    std::string statusText;

    if (type && strcmp(type, "unavailable") == 0) {
        show = "unavailable";
    } else {
        xmpp_stanza_t* showEl = xmpp_stanza_get_child_by_name(stanza, "show");
        if (showEl) {
            const char* s = xmpp_stanza_get_text(showEl);
            if (s) show = s;
        }
    }

    xmpp_stanza_t* statusEl = xmpp_stanza_get_child_by_name(stanza, "status");
    if (statusEl) {
        const char* s = xmpp_stanza_get_text(statusEl);
        if (s) statusText = s;
    }

    client->onPresence(from ? from : "", statusText, show);

    xmpp_stanza_t* xMuc = xmpp_stanza_get_child_by_ns(stanza, "http://jabber.org/protocol/muc#user");
    if (xMuc && from) {
        std::string roomJid = jidBare(from);
        std::string nick;
        auto slash = std::string(from).find('/');
        if (slash != std::string::npos) {
            nick.assign(from + slash + 1);
        }

        xmpp_stanza_t* item = xmpp_stanza_get_child_by_name(xMuc, "item");
        std::string role, affiliation, participantJid;
        if (item) {
            const char* r = xmpp_stanza_get_attribute(item, "role");
            if (r) role = r;
            const char* a = xmpp_stanza_get_attribute(item, "affiliation");
            if (a) affiliation = a;
            const char* j = xmpp_stanza_get_attribute(item, "jid");
            if (j) participantJid = j;
        }

        bool joined = true;
        if (type && strcmp(type, "unavailable") == 0) {
            joined = false;
        }

        client->onMucParticipant(roomJid, participantJid, nick, role, affiliation, joined);

        xmpp_stanza_t* statusCodes = xmpp_stanza_get_children(xMuc);
        while (statusCodes) {
            const char* scName = xmpp_stanza_get_name(statusCodes);
            if (scName && strcmp(scName, "status") == 0) {
                const char* code = xmpp_stanza_get_attribute(statusCodes, "code");
                if (code) {
                    int icode = std::atoi(code);
                    if (icode == 201) {
                        client->requestRoomConfig(roomJid);
                    }
                    if (icode == 110) {
                        std::lock_guard<std::mutex> lock(client->mucMutex);
                        client->mucRooms[roomJid].ownNickname = nick;
                    }
                }
            }
            statusCodes = xmpp_stanza_get_next(statusCodes);
        }

        if (nick == client->getOwnJid()) {
            std::lock_guard<std::mutex> lock(client->mucMutex);
            client->mucRooms[roomJid].isJoined = joined;
            client->mucRooms[roomJid].ownNickname = nick;
        }
    }

    return 1;
}

// ====== MAM (XEP-0313) ======

std::string XmppClient::generateId() {
    std::lock_guard<std::mutex> lock(callbackMutex);
    return "mam_" + std::to_string(++iqCounter);
}

void XmppClient::requestMam(const std::string& chatJid, const std::string& beforeId, int limit) {
    if (!conn || !ctx) return;

    XmppMamQuery query;
    query.chatJid = chatJid;
    query.beforeId = beforeId;
    query.limit = limit;

    std::string id = generateId();
    {
        std::lock_guard<std::mutex> lock(mamMutex);
        mamQueries[id] = query;
    }

    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "set");
    xmpp_stanza_set_id(iq, id.c_str());

    xmpp_stanza_t* queryEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(queryEl, "query");
    xmpp_stanza_set_ns(queryEl, "urn:xmpp:mam:2");

    // x jabbber:x:data
    xmpp_stanza_t* x = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(x, "x");
    xmpp_stanza_set_ns(x, "jabber:x:data");
    xmpp_stanza_set_type(x, "submit");

    // field FORM_TYPE
    xmpp_stanza_t* ft = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(ft, "field");
    xmpp_stanza_set_attribute(ft, "var", "FORM_TYPE");
    xmpp_stanza_set_type(ft, "hidden");
    xmpp_stanza_t* ftv = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(ftv, "value");
    xmpp_stanza_set_text(ftv, "urn:xmpp:mam:2");
    xmpp_stanza_add_child(ft, ftv);
    xmpp_stanza_release(ftv);
    xmpp_stanza_add_child(x, ft);
    xmpp_stanza_release(ft);

    // field with
    xmpp_stanza_t* fw = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(fw, "field");
    xmpp_stanza_set_attribute(fw, "var", "with");
    xmpp_stanza_t* fwv = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(fwv, "value");
    xmpp_stanza_set_text(fwv, chatJid.c_str());
    xmpp_stanza_add_child(fw, fwv);
    xmpp_stanza_release(fwv);
    xmpp_stanza_add_child(x, fw);
    xmpp_stanza_release(fw);

    if (!beforeId.empty()) {
        xmpp_stanza_t* fb = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(fb, "field");
        xmpp_stanza_set_attribute(fb, "var", "before-id");
        xmpp_stanza_t* fbv = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(fbv, "value");
        xmpp_stanza_set_text(fbv, beforeId.c_str());
        xmpp_stanza_add_child(fb, fbv);
        xmpp_stanza_release(fbv);
        xmpp_stanza_add_child(x, fb);
        xmpp_stanza_release(fb);
    }

    xmpp_stanza_add_child(queryEl, x);
    xmpp_stanza_release(x);

    // RSM set
    xmpp_stanza_t* setEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(setEl, "set");
    xmpp_stanza_set_ns(setEl, "http://jabber.org/protocol/rsm");
    xmpp_stanza_t* maxEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(maxEl, "max");
    xmpp_stanza_set_text(maxEl, std::to_string(limit).c_str());
    xmpp_stanza_add_child(setEl, maxEl);
    xmpp_stanza_release(maxEl);
    if (!beforeId.empty()) {
        xmpp_stanza_t* beforeEl = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(beforeEl, "before");
        xmpp_stanza_set_text(beforeEl, beforeId.c_str());
        xmpp_stanza_add_child(setEl, beforeEl);
        xmpp_stanza_release(beforeEl);
    }
    xmpp_stanza_add_child(queryEl, setEl);
    xmpp_stanza_release(setEl);

    xmpp_stanza_add_child(iq, queryEl);
    xmpp_stanza_release(queryEl);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

void XmppClient::requestMamPage(const std::string& chatJid, const std::string& afterId, int limit) {
    if (!conn || !ctx) return;

    XmppMamQuery query;
    query.chatJid = chatJid;
    query.afterId = afterId;
    query.limit = limit;

    std::string id = generateId();
    {
        std::lock_guard<std::mutex> lock(mamMutex);
        mamQueries[id] = query;
    }

    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "set");
    xmpp_stanza_set_id(iq, id.c_str());

    xmpp_stanza_t* queryEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(queryEl, "query");
    xmpp_stanza_set_ns(queryEl, "urn:xmpp:mam:2");

    xmpp_stanza_t* x = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(x, "x");
    xmpp_stanza_set_ns(x, "jabber:x:data");
    xmpp_stanza_set_type(x, "submit");

    xmpp_stanza_t* ft = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(ft, "field");
    xmpp_stanza_set_attribute(ft, "var", "FORM_TYPE");
    xmpp_stanza_set_type(ft, "hidden");
    xmpp_stanza_t* ftv = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(ftv, "value");
    xmpp_stanza_set_text(ftv, "urn:xmpp:mam:2");
    xmpp_stanza_add_child(ft, ftv);
    xmpp_stanza_release(ftv);
    xmpp_stanza_add_child(x, ft);
    xmpp_stanza_release(ft);

    xmpp_stanza_t* fw = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(fw, "field");
    xmpp_stanza_set_attribute(fw, "var", "with");
    xmpp_stanza_t* fwv = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(fwv, "value");
    xmpp_stanza_set_text(fwv, chatJid.c_str());
    xmpp_stanza_add_child(fw, fwv);
    xmpp_stanza_release(fwv);
    xmpp_stanza_add_child(x, fw);
    xmpp_stanza_release(fw);

    xmpp_stanza_add_child(queryEl, x);
    xmpp_stanza_release(x);

    xmpp_stanza_t* setEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(setEl, "set");
    xmpp_stanza_set_ns(setEl, "http://jabber.org/protocol/rsm");
    xmpp_stanza_t* maxEl = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(maxEl, "max");
    xmpp_stanza_set_text(maxEl, std::to_string(limit).c_str());
    xmpp_stanza_add_child(setEl, maxEl);
    xmpp_stanza_release(maxEl);
    if (!afterId.empty()) {
        xmpp_stanza_t* afterEl = xmpp_stanza_new(ctx);
        xmpp_stanza_set_name(afterEl, "after");
        xmpp_stanza_set_text(afterEl, afterId.c_str());
        xmpp_stanza_add_child(setEl, afterEl);
        xmpp_stanza_release(afterEl);
    }
    xmpp_stanza_add_child(queryEl, setEl);
    xmpp_stanza_release(setEl);

    xmpp_stanza_add_child(iq, queryEl);
    xmpp_stanza_release(queryEl);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

void XmppClient::onMamResult(const std::string& queryChatJid, const std::string& json) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    if (eventCallback) {
        eventCallback(XmppEventType::MAM_MESSAGE, json);
    }
}

int XmppClient::s_onMamIq(xmpp_conn_t* /*conn*/, xmpp_stanza_t* stanza, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);

    const char* type = xmpp_stanza_get_type(stanza);
    if (!type) return 0;

    const char* id = xmpp_stanza_get_id(stanza);

    XmppMamQuery query;
    bool hasQuery = false;
    if (id) {
        std::lock_guard<std::mutex> lock(client->mamMutex);
        auto it = client->mamQueries.find(id);
        if (it != client->mamQueries.end()) {
            query = it->second;
            hasQuery = true;
        }
    }

    if (strcmp(type, "result") == 0) {
        xmpp_stanza_t* queryEl = xmpp_stanza_get_child_by_ns(stanza, "urn:xmpp:mam:2");
        if (!queryEl) {
            queryEl = xmpp_stanza_get_child_by_name(stanza, "query");
        }

        if (queryEl) {
            int msgCount = 0;
            XmppMamPage page;

            xmpp_stanza_t* setEl = xmpp_stanza_get_child_by_ns(queryEl, "http://jabber.org/protocol/rsm");
            if (!setEl) {
                setEl = xmpp_stanza_get_child_by_name(queryEl, "set");
            }
            if (setEl) {
                xmpp_stanza_t* firstEl = xmpp_stanza_get_child_by_name(setEl, "first");
                if (firstEl) {
                    const char* p = xmpp_stanza_get_text(firstEl);
                    if (p) page.first = p;
                }
                xmpp_stanza_t* lastEl = xmpp_stanza_get_child_by_name(setEl, "last");
                if (lastEl) {
                    const char* p = xmpp_stanza_get_text(lastEl);
                    if (p) page.last = p;
                }
                xmpp_stanza_t* countEl = xmpp_stanza_get_child_by_name(setEl, "count");
                if (countEl) {
                    const char* p = xmpp_stanza_get_text(countEl);
                    if (p) page.count = std::atoi(p);
                }
            }

            auto now = std::chrono::system_clock::now();
            int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count();

            xmpp_stanza_t* child = xmpp_stanza_get_children(queryEl);
            while (child) {
                const char* cname = xmpp_stanza_get_name(child);
                if (cname && strcmp(cname, "forwarded") == 0) {
                    xmpp_stanza_t* delay = xmpp_stanza_get_child_by_name(child, "delay");
                    if (delay) {
                        const char* s = xmpp_stanza_get_attribute(delay, "stamp");
                        if (s) {
                            std::string stamp(s);
                            ts = 0;
                            if (stamp.size() >= 10) {
                                ts = std::atoll(stamp.substr(0, 10).c_str()) * 1000;
                            }
                        }
                    }

                    xmpp_stanza_t* msg = xmpp_stanza_get_child_by_name(child, "message");
                    if (!msg) {
                        child = xmpp_stanza_get_next(child);
                        continue;
                    }

                    const char* from = xmpp_stanza_get_attribute(msg, "from");
                    const char* mtype = xmpp_stanza_get_type(msg);

                    xmpp_stanza_t* bodyEl = xmpp_stanza_get_child_by_name(msg, "body");
                    const char* bodyText = nullptr;
                    if (bodyEl) {
                        bodyText = xmpp_stanza_get_text(bodyEl);
                    }

                    if (bodyText && from) {
                        std::ostringstream json;
                        json << R"({"chatJid":")" << escapeJson(hasQuery ? query.chatJid : from)
                             << R"(","senderJid":")" << escapeJson(from)
                             << R"(","senderName":")" << escapeJson(from)
                             << R"(","body":")" << escapeJson(bodyText) << R"(")"
                             << R"(,"timestamp":)" << ts
                             << R"(,"isOutgoing":false,"type":")"
                             << escapeJson(mtype ? mtype : "chat") << R"("})";
                        client->onMamResult(hasQuery ? query.chatJid : from, json.str());
                        msgCount++;
                    }

                    child = xmpp_stanza_get_next(child);
                    continue;
                }
                if (cname && strcmp(cname, "message") == 0) {
                    const char* from = xmpp_stanza_get_attribute(child, "from");
                    const char* mtype = xmpp_stanza_get_type(child);

                    xmpp_stanza_t* bodyEl = xmpp_stanza_get_child_by_name(child, "body");
                    const char* bodyText = nullptr;
                    if (bodyEl) {
                        bodyText = xmpp_stanza_get_text(bodyEl);
                    }

                    if (bodyText && from) {
                        std::ostringstream json;
                        json << R"({"chatJid":")" << escapeJson(hasQuery ? query.chatJid : from)
                             << R"(","senderJid":")" << escapeJson(from)
                             << R"(","senderName":")" << escapeJson(from)
                             << R"(","body":")" << escapeJson(bodyText) << R"(")"
                             << R"(,"timestamp":)" << ts
                             << R"(,"isOutgoing":false,"type":")"
                             << escapeJson(mtype ? mtype : "chat") << R"("})";
                        client->onMamResult(hasQuery ? query.chatJid : from, json.str());
                        msgCount++;
                    }
                }
                child = xmpp_stanza_get_next(child);
            }

            page.complete = msgCount == 0 || page.last.empty()
                || (setEl && msgCount < page.count);

            if (page.complete) {
                std::ostringstream pgJson;
                pgJson << R"({"chatJid":")" << escapeJson(hasQuery ? query.chatJid : "")
                       << R"(","first":")" << escapeJson(page.first)
                       << R"(","last":")" << escapeJson(page.last)
                       << R"(,"count":)" << page.count
                       << R"(,"complete":true})";
                std::lock_guard<std::mutex> lock(client->callbackMutex);
                if (client->eventCallback) {
                    client->eventCallback(XmppEventType::MAM_COMPLETE, pgJson.str());
                }
            }
        }
    } else if (strcmp(type, "error") == 0) {
        std::lock_guard<std::mutex> lock(client->callbackMutex);
        if (client->eventCallback) {
            std::ostringstream json;
            json << R"({"chatJid":")" << escapeJson(hasQuery ? query.chatJid : "")
                 << R"(","error":"mam_query_failed"})";
            client->eventCallback(XmppEventType::MAM_COMPLETE, json.str());
        }
    }

    if (id && hasQuery) {
        std::lock_guard<std::mutex> lock(client->mamMutex);
        client->mamQueries.erase(id);
    }

    return 1;
}

// ====== MUC (XEP-0045) ======

void XmppClient::requestRoomInfo(const std::string& roomJid) {
    if (!conn || !ctx) return;

    std::string id = generateId();
    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "get");
    xmpp_stanza_set_attribute(iq, "to", roomJid.c_str());
    xmpp_stanza_set_id(iq, id.c_str());

    xmpp_stanza_t* query = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(query, "query");
    xmpp_stanza_set_ns(query, "http://jabber.org/protocol/disco#info");
    xmpp_stanza_add_child(iq, query);
    xmpp_stanza_release(query);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

void XmppClient::requestRoomConfig(const std::string& roomJid) {
    if (!conn || !ctx) return;

    std::string id = generateId();
    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "get");
    xmpp_stanza_set_attribute(iq, "to", roomJid.c_str());
    xmpp_stanza_set_id(iq, id.c_str());

    xmpp_stanza_t* query = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(query, "query");
    xmpp_stanza_set_ns(query, "http://jabber.org/protocol/muc#owner");
    xmpp_stanza_add_child(iq, query);
    xmpp_stanza_release(query);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

std::vector<XmppMucRoom> XmppClient::getRooms() const {
    std::lock_guard<std::mutex> lock(mucMutex);
    std::vector<XmppMucRoom> rooms;
    for (const auto& pair : mucRooms) {
        rooms.push_back(pair.second);
    }
    return rooms;
}

XmppMucRoom XmppClient::getRoomInfo(const std::string& roomJid) const {
    std::lock_guard<std::mutex> lock(mucMutex);
    auto it = mucRooms.find(roomJid);
    if (it != mucRooms.end()) {
        return it->second;
    }
    XmppMucRoom empty;
    empty.jid = roomJid;
    return empty;
}

void XmppClient::onMucSubject(const std::string& roomJid, const std::string& subject,
                               const std::string& fromNick) {
    {
        std::lock_guard<std::mutex> lock(mucMutex);
        auto& room = mucRooms[roomJid];
        room.jid = roomJid;
        room.subject = subject;
        if (!room.isJoined) room.isJoined = true;
    }
    std::lock_guard<std::mutex> lock(callbackMutex);
    if (eventCallback) {
        std::ostringstream json;
        json << R"({"roomJid":")" << escapeJson(roomJid)
             << R"(","subject":")" << escapeJson(subject)
             << R"(","nickname":")" << escapeJson(fromNick) << R"("})";
        eventCallback(XmppEventType::PRESENCE_CHANGED, json.str());
    }
}

void XmppClient::onMucParticipant(const std::string& roomJid, const std::string& participantJid,
                                   const std::string& nick, const std::string& role,
                                   const std::string& affiliation, bool joined) {
    {
        std::lock_guard<std::mutex> lock(mucMutex);
        auto& room = mucRooms[roomJid];
        room.jid = roomJid;
        if (!room.isJoined) room.isJoined = true;

        bool found = false;
        for (auto& p : room.participants) {
            if (p.nickname == nick) {
                p.role = role;
                p.affiliation = affiliation;
                if (!participantJid.empty()) p.jid = participantJid;
                if (!joined) {
                    p.role = "none";
                }
                found = true;
                break;
            }
        }
        if (!found && joined) {
            XmppMucParticipant p;
            p.jid = participantJid;
            p.nickname = nick;
            p.role = role;
            p.affiliation = affiliation;
            room.participants.push_back(std::move(p));
        }
    }
}

// ====== VCard (XEP-0054) + Avatars (XEP-0153) ======

void XmppClient::requestVCard(const std::string& jid) {
    if (!conn || !ctx) return;

    std::string id = generateId();
    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "get");
    xmpp_stanza_set_id(iq, id.c_str());
    if (!jid.empty()) {
        xmpp_stanza_set_attribute(iq, "to", jid.c_str());
    }

    xmpp_stanza_t* vcard = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(vcard, "vCard");
    xmpp_stanza_set_ns(vcard, "vcard-temp");
    xmpp_stanza_add_child(iq, vcard);
    xmpp_stanza_release(vcard);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

std::string XmppClient::getAvatarHash(const std::string& jid) const {
    std::lock_guard<std::mutex> lock(avatarMutex);
    auto it = avatarHashes.find(jidBare(jid));
    return it != avatarHashes.end() ? it->second : "";
}

void XmppClient::onVCard(const std::string& jid, const std::string& fullName,
                          const std::string& photoType, const std::string& photoData) {
    std::string bareJid = jid.empty() ? ownJid : jidBare(jid);

    {
        std::lock_guard<std::mutex> lock(avatarMutex);
        if (!fullName.empty()) {
            displayNames[bareJid] = fullName;
        }
        if (!photoData.empty()) {
            std::hash<std::string> hasher;
            avatarHashes[bareJid] = std::to_string(hasher(photoData));
        }
    }

    std::lock_guard<std::mutex> lock(callbackMutex);
    if (eventCallback) {
        std::ostringstream json;
        json << R"({"jid":")" << escapeJson(bareJid)
             << R"(","fullName":")" << escapeJson(fullName)
             << R"(","photoType":")" << escapeJson(photoType)
             << R"(","hasPhoto":)" << (photoData.empty() ? "false" : "true") << "}";
        eventCallback(XmppEventType::PRESENCE_CHANGED, json.str());
    }
}

int XmppClient::s_onVCardIq(xmpp_conn_t* /*conn*/, xmpp_stanza_t* stanza, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);

    const char* type = xmpp_stanza_get_type(stanza);
    if (!type || strcmp(type, "result") != 0) return 1;

    const char* from = xmpp_stanza_get_attribute(stanza, "from");
    std::string jid = from ? from : "";

    xmpp_stanza_t* vcard = xmpp_stanza_get_child_by_ns(stanza, "vcard-temp");
    if (!vcard) {
        vcard = xmpp_stanza_get_child_by_name(stanza, "vCard");
    }
    if (!vcard) return 1;

    std::string fullName;
    xmpp_stanza_t* fn = xmpp_stanza_get_child_by_name(vcard, "FN");
    if (fn) {
        const char* t = xmpp_stanza_get_text(fn);
        if (t) fullName = t;
    }
    if (fullName.empty()) {
        xmpp_stanza_t* n = xmpp_stanza_get_child_by_name(vcard, "N");
        if (n) {
            xmpp_stanza_t* given = xmpp_stanza_get_child_by_name(n, "GIVEN");
            xmpp_stanza_t* family = xmpp_stanza_get_child_by_name(n, "FAMILY");
            if (given && family) {
                const char* g = xmpp_stanza_get_text(given);
                const char* f = xmpp_stanza_get_text(family);
                if (g && f) fullName = std::string(g) + " " + std::string(f);
            } else if (given) {
                const char* g = xmpp_stanza_get_text(given);
                if (g) fullName = g;
            }
        }
    }

    std::string photoType, photoData;
    xmpp_stanza_t* photo = xmpp_stanza_get_child_by_name(vcard, "PHOTO");
    if (photo) {
        xmpp_stanza_t* typeEl = xmpp_stanza_get_child_by_name(photo, "TYPE");
        if (typeEl) {
            const char* t = xmpp_stanza_get_text(typeEl);
            if (t) photoType = t;
        }
        xmpp_stanza_t* binval = xmpp_stanza_get_child_by_name(photo, "BINVAL");
        if (binval) {
            const char* t = xmpp_stanza_get_text(binval);
            if (t) photoData = t;
        }
    }

    client->onVCard(jid, fullName, photoType, photoData);

    return 1;
}

// ====== HTTP Upload (XEP-0363) ======

void XmppClient::requestUploadSlot(const std::string& serviceJid, const std::string& filename,
                                    int64_t fileSize, const std::string& contentType) {
    if (!conn || !ctx) return;

    std::string id = generateId();
    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "get");
    xmpp_stanza_set_id(iq, id.c_str());
    if (!serviceJid.empty()) {
        xmpp_stanza_set_attribute(iq, "to", serviceJid.c_str());
    }

    xmpp_stanza_t* request = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(request, "request");
    xmpp_stanza_set_ns(request, "urn:xmpp:http:upload:0");
    xmpp_stanza_set_attribute(request, "filename", filename.c_str());
    xmpp_stanza_set_attribute(request, "size", std::to_string(fileSize).c_str());
    if (!contentType.empty()) {
        xmpp_stanza_set_attribute(request, "content-type", contentType.c_str());
    }
    xmpp_stanza_add_child(iq, request);
    xmpp_stanza_release(request);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);
}

int XmppClient::s_onUploadIq(xmpp_conn_t* /*conn*/, xmpp_stanza_t* stanza, void* userdata) {
    auto* client = static_cast<XmppClient*>(userdata);

    const char* type = xmpp_stanza_get_type(stanza);
    if (!type) return 1;

    xmpp_stanza_t* slot = xmpp_stanza_get_child_by_ns(stanza, "urn:xmpp:http:upload:0");
    if (!slot) {
        slot = xmpp_stanza_get_child_by_name(stanza, "slot");
    }
    if (!slot) return 1;

    std::string putUrl, getUrl;
    xmpp_stanza_t* putEl = xmpp_stanza_get_child_by_name(slot, "put");
    if (putEl) {
        const char* url = xmpp_stanza_get_attribute(putEl, "url");
        if (url) putUrl = url;
    }
    xmpp_stanza_t* getEl = xmpp_stanza_get_child_by_name(slot, "get");
    if (getEl) {
        const char* url = xmpp_stanza_get_attribute(getEl, "url");
        if (url) getUrl = url;
    }

    {
        std::lock_guard<std::mutex> lock(client->callbackMutex);
        if (client->eventCallback) {
            std::ostringstream json;
            if (strcmp(type, "result") == 0 && !putUrl.empty() && !getUrl.empty()) {
                json << R"({"putUrl":")" << escapeJson(putUrl)
                     << R"(","getUrl":")" << escapeJson(getUrl) << R"("})";
            } else {
                json << R"({"error":"upload_slot_failed"})";
            }
            client->eventCallback(XmppEventType::ERROR, json.str());
        }
    }

    return 1;
}

// ====== Message Carbons (XEP-0280) ======

void XmppClient::enableCarbons() {
    if (!conn || !ctx) return;

    xmpp_stanza_t* iq = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "set");
    xmpp_stanza_set_id(iq, "enable_carbons");

    xmpp_stanza_t* enable = xmpp_stanza_new(ctx);
    xmpp_stanza_set_name(enable, "enable");
    xmpp_stanza_set_ns(enable, "urn:xmpp:carbons:2");
    xmpp_stanza_add_child(iq, enable);
    xmpp_stanza_release(enable);

    xmpp_send(conn, iq);
    xmpp_stanza_release(iq);

    LOGD("Carbones enabled");
}

} // namespace pcp

#endif // PCP_HAS_XMPP
