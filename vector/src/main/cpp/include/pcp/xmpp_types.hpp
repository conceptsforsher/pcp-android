#ifndef PCP_XMPP_TYPES_HPP
#define PCP_XMPP_TYPES_HPP

#ifdef PCP_HAS_XMPP

#include <string>
#include <cstdint>

namespace pcp {

enum class XmppConnectionState {
    DISCONNECTED, CONNECTING, CONNECTED, AUTHENTICATED, ERROR
};

struct XmppUser {
    std::string jid;
    std::string nickname;
    std::string status;
    std::string avatarHash;
    bool isOnline = false;
};

struct XmppChat {
    std::string jid;
    std::string name;
    std::string lastMessage;
    int64_t lastMessageTime = 0;
    int unreadCount = 0;
    bool isGroupChat = false;
};

struct XmppMessage {
    std::string id;
    std::string chatJid;
    std::string senderJid;
    std::string senderName;
    std::string body;
    int64_t timestamp = 0;
    bool isOutgoing = false;
    std::string type;
};

struct XmppAccount {
    std::string jid;
    std::string password;
    std::string server;
    int port = 5222;
    std::string resource = "PCP";
    bool useTls = true;
};

enum class XmppEventType {
    CONNECTION_CHANGED,
    AUTH_CHANGED,
    MESSAGE_RECEIVED,
    PRESENCE_CHANGED,
    ROSTER_UPDATED,
    ERROR
};

} // namespace pcp

#endif // PCP_HAS_XMPP
#endif // PCP_XMPP_TYPES_HPP
