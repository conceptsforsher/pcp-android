#ifndef PCP_XMPP_TYPES_HPP
#define PCP_XMPP_TYPES_HPP

#ifdef PCP_HAS_XMPP

#include <string>
#include <cstdint>
#include <vector>

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

struct XmppMamPage {
    std::string first;
    std::string last;
    int count = 0;
    bool complete = false;
};

struct XmppMamQuery {
    std::string chatJid;
    std::string beforeId;
    std::string afterId;
    int limit = 50;
};

struct XmppMucParticipant {
    std::string jid;
    std::string nickname;
    std::string role;
    std::string affiliation;
};

struct XmppMucRoom {
    std::string jid;
    std::string name;
    std::string subject;
    std::string ownNickname;
    std::vector<XmppMucParticipant> participants;
    bool isJoined = false;
};

struct XmppRosterItem {
    std::string jid;
    std::string name;
    std::string subscription;
    std::vector<std::string> groups;
};

struct XmppUploadSlot {
    std::string putUrl;
    std::string getUrl;
    std::string error;
};

enum class XmppEventType {
    CONNECTION_CHANGED,
    AUTH_CHANGED,
    MESSAGE_RECEIVED,
    MAM_MESSAGE,
    MAM_COMPLETE,
    PRESENCE_CHANGED,
    ROSTER_UPDATED,
    ERROR
};

} // namespace pcp

#endif // PCP_HAS_XMPP
#endif // PCP_XMPP_TYPES_HPP
