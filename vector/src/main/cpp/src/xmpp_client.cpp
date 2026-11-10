#ifdef PCP_HAS_XMPP

extern "C" {
#include <strophe.h>
}

#include "pcp/xmpp_client.hpp"
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

    xmpp_stanza_t* bodyEl = xmpp_stanza_new_child(msg, "body");
    if (bodyEl) {
        xmpp_stanza_t* text = xmpp_stanza_new(ctx);
        xmpp_stanza_set_text(text, body.c_str());
        xmpp_stanza_add_child(bodyEl, text);
        xmpp_stanza_release(text);
    }

    xmpp_stanza_send(conn, msg);
    xmpp_stanza_release(msg);

    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            std::ostringstream json;
            json << R"({"chatJid":")" << toJid
                 << R"(","senderJid":")" << ownJid
                 << R"(","senderName":")" << ownJid
                 << R"(","body":)" << '"' << body << '"'
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
        xmpp_stanza_t* showEl = xmpp_stanza_new_child(pres, "show");
        if (showEl) {
            xmpp_stanza_t* text = xmpp_stanza_new(ctx);
            xmpp_stanza_set_text(text, show.c_str());
            xmpp_stanza_add_child(showEl, text);
            xmpp_stanza_release(text);
        }
    }

    if (!status.empty()) {
        xmpp_stanza_t* statusEl = xmpp_stanza_new_child(pres, "status");
        if (statusEl) {
            xmpp_stanza_t* text = xmpp_stanza_new(ctx);
            xmpp_stanza_set_text(text, status.c_str());
            xmpp_stanza_add_child(statusEl, text);
            xmpp_stanza_release(text);
        }
    }

    xmpp_stanza_send(conn, pres);
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
    xmpp_stanza_t* state = xmpp_stanza_new_child(msg, childName);
    if (state) {
        xmpp_stanza_set_ns(state, "http://jabber.org/protocol/chatstates");
    }

    xmpp_stanza_send(conn, msg);
    xmpp_stanza_release(msg);
}

void XmppClient::joinRoom(const std::string& roomJid) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");

    std::string toAddr = roomJid + "/" + account.resource;
    xmpp_stanza_set_attribute(pres, "to", toAddr.c_str());

    xmpp_stanza_t* x = xmpp_stanza_new_child(pres, "x");
    if (x) {
        xmpp_stanza_set_ns(x, "http://jabber.org/protocol/muc");
    }

    xmpp_stanza_send(conn, pres);
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

    xmpp_stanza_send(conn, pres);
    xmpp_stanza_release(pres);
}

void XmppClient::setNickname(const std::string& roomJid, const std::string& nick) {
    if (!conn || !ctx) return;

    xmpp_stanza_t* pres = xmpp_stanza_new(ctx);
    if (!pres) return;

    xmpp_stanza_set_name(pres, "presence");

    std::string toAddr = roomJid + "/" + nick;
    xmpp_stanza_set_attribute(pres, "to", toAddr.c_str());

    xmpp_stanza_send(conn, pres);
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
                            const std::string& type) {
    auto now = std::chrono::system_clock::now();
    int64_t ts = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();

    {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (eventCallback) {
            std::ostringstream json;
            json << R"({"chatJid":")" << from
                 << R"(","senderJid":")" << from
                 << R"(","senderName":")" << from
                 << R"(","body":)" << '"' << body << '"'
                 << R"(,"timestamp":)" << ts
                 << R"(,"isOutgoing":false,"type":")" << type << R"("})";
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
            json << R"({"jid":")" << from
                 << R"(","status":")" << status
                 << R"(","show":")" << show << R"("})";
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

    xmpp_stanza_t* bodyEl = xmpp_stanza_get_child_by_name(stanza, "body");
    const char* bodyText = nullptr;
    if (bodyEl) {
        bodyText = xmpp_stanza_get_text(bodyEl);
    }

    if (bodyText && from) {
        client->onMessage(
            from,
            bodyText,
            type ? type : "chat"
        );
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

    return 1;
}

} // namespace pcp

#endif // PCP_HAS_XMPP
