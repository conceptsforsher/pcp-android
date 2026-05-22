#ifndef PROGRESSIVE_XMPP_CLIENT_HPP
#define PROGRESSIVE_XMPP_CLIENT_HPP

#ifdef PROGRESSIVE_HAS_XMPP

#include "progressive/xmpp_types.hpp"
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <memory>
#include <unordered_map>

struct _xmpp_ctx_t;
struct _xmpp_conn_t;
struct _xmpp_stanza_t;
struct _xmpp_stream_error_t;
typedef struct _xmpp_ctx_t xmpp_ctx_t;
typedef struct _xmpp_conn_t xmpp_conn_t;
typedef struct _xmpp_stanza_t xmpp_stanza_t;
typedef struct _xmpp_stream_error_t xmpp_stream_error_t;
typedef enum {
    XMPP_CONN_CONNECT,
    XMPP_CONN_DISCONNECT,
    XMPP_CONN_FAIL
} xmpp_conn_event_t;

namespace progressive {

class XmppClient {
public:
    XmppClient();
    ~XmppClient();

    XmppClient(const XmppClient&) = delete;
    XmppClient& operator=(const XmppClient&) = delete;

    bool connect(const XmppAccount& account);
    void disconnect();
    bool isConnected() const;
    bool isAuthenticated() const;
    std::string getOwnJid() const;

    void sendMessage(const std::string& toJid, const std::string& body);
    void sendPresence(const std::string& status, const std::string& show);
    void sendTypingNotification(const std::string& toJid, bool isTyping);
    void joinRoom(const std::string& roomJid);
    void leaveRoom(const std::string& roomJid);
    void setNickname(const std::string& roomJid, const std::string& nick);

    std::vector<XmppUser> getRoster() const;
    std::vector<XmppChat> getChats() const;

    void requestMam(const std::string& chatJid, const std::string& beforeId = "", int limit = 50);
    void requestMamPage(const std::string& chatJid, const std::string& afterId = "", int limit = 50);

    void requestRoomInfo(const std::string& roomJid);
    void requestRoomConfig(const std::string& roomJid);
    std::vector<XmppMucRoom> getRooms() const;
    XmppMucRoom getRoomInfo(const std::string& roomJid) const;

    void requestVCard(const std::string& jid);
    std::string getAvatarHash(const std::string& jid) const;

    void requestUploadSlot(const std::string& serviceJid, const std::string& filename,
                           int64_t fileSize, const std::string& contentType);

    void enableCarbons();

    using EventCallback = std::function<void(XmppEventType, const std::string& json)>;
    void setEventCallback(EventCallback cb);

    void runOnce(int timeoutMs = 100);

private:
    void eventLoop();
    void onConnect(xmpp_conn_t* conn, int status);
    void onMessage(const std::string& from, const std::string& body, const std::string& type,
                   bool isMuc = false);
    void onPresence(const std::string& from, const std::string& status, const std::string& show);
    void onMamResult(const std::string& queryChatJid, const std::string& json);
    void onMucSubject(const std::string& roomJid, const std::string& subject, const std::string& fromNick);
    void onMucParticipant(const std::string& roomJid, const std::string& participantJid,
                          const std::string& nick, const std::string& role,
                          const std::string& affiliation, bool joined);
    void onVCard(const std::string& jid, const std::string& fullName,
                 const std::string& photoType, const std::string& photoData);

    std::string generateId();

    static void s_onConnect(xmpp_conn_t* conn, xmpp_conn_event_t status, int error,
                            xmpp_stream_error_t* streamError, void* userdata);
    static int s_onMessage(xmpp_conn_t* conn, xmpp_stanza_t* stanza, void* userdata);
    static int s_onPresence(xmpp_conn_t* conn, xmpp_stanza_t* stanza, void* userdata);
    static int s_onMamIq(xmpp_conn_t* conn, xmpp_stanza_t* stanza, void* userdata);
    static int s_onVCardIq(xmpp_conn_t* conn, xmpp_stanza_t* stanza, void* userdata);
    static int s_onUploadIq(xmpp_conn_t* conn, xmpp_stanza_t* stanza, void* userdata);

    xmpp_ctx_t* ctx = nullptr;
    xmpp_conn_t* conn = nullptr;

    std::thread eventThread;
    std::atomic<bool> running{false};
    std::atomic<bool> authenticated{false};

    std::string ownJid;
    XmppAccount account;

    mutable std::mutex rosterMutex;
    std::vector<XmppUser> roster;

    mutable std::mutex chatsMutex;
    std::vector<XmppChat> chats;

    EventCallback eventCallback;
    mutable std::mutex callbackMutex;

    mutable std::mutex mamMutex;
    std::unordered_map<std::string, XmppMamQuery> mamQueries;
    int64_t iqCounter = 0;

    mutable std::mutex mucMutex;
    std::unordered_map<std::string, XmppMucRoom> mucRooms;

    mutable std::mutex avatarMutex;
    std::unordered_map<std::string, std::string> avatarHashes;
    std::unordered_map<std::string, std::string> displayNames;
};

} // namespace progressive

#endif // PROGRESSIVE_HAS_XMPP
#endif // PROGRESSIVE_XMPP_CLIENT_HPP
