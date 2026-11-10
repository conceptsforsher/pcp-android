#include "pcp/room_summary_formatter.hpp"
#include "pcp/json_parser.hpp"
#include <sstream>
#include <chrono>
#include <ctime>

namespace pcp {

static std::string formatTimestamp(int64_t ms) {
    time_t t = ms / 1000;
    int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int64_t diff = now - ms;
    if (diff < 60000) return "Now";
    if (diff < 3600000) return std::to_string(diff / 60000) + "m";
    if (diff < 86400000) {
        struct tm tm; localtime_r(&t, &tm);
        char buf[8]; strftime(buf, sizeof(buf), "%H:%M", &tm);
        return buf;
    }
    if (diff < 172800000) return "Yesterday";
    if (diff < 604800000) return std::to_string(diff / 86400000) + "d";
    struct tm tm; localtime_r(&t, &tm);
    char buf[16]; strftime(buf, sizeof(buf), "%b %d", &tm);
    return buf;
}

RoomSummaryDisplay formatRoomSummary(const std::string& roomId, const std::string& name,
                                       const std::string& body, const std::string& sender,
                                       int64_t ts, int notifCount, int hlCount,
                                       bool direct, const std::string& typing) {
    RoomSummaryDisplay d;
    d.roomId = roomId;
    d.displayName = name;
    d.lastMessage = body;
    d.lastMessageSender = sender;
    d.lastMessageTs = ts;
    d.formattedTime = formatTimestamp(ts);
    d.notificationCount = notifCount;
    d.highlightCount = hlCount;
    d.isDirect = direct;
    d.typingText = typing;
    return d;
}

std::string formatNotificationBadge(int count, int hlCount) {
    if (count <= 0) return "";
    if (count >= 1000) return "999+";
    return std::to_string(count);
}

std::string formatLastMessagePreview(const std::string& body, const std::string& sender,
                                       bool isOwn) {
    std::string prefix = isOwn ? "You: " : "";
    std::string preview = body;
    if (preview.size() > 80) preview = preview.substr(0, 77) + "...";
    return prefix + preview;
}

std::string formatRoomListTyping(const std::vector<std::string>& ids,
                                   const std::vector<std::string>& names, int max) {
    if (ids.empty()) return "";
    if (ids.size() == 1) {
        std::string n = names.empty() ? ids[0] : names[0];
        return n + " is typing...";
    }
    if (ids.size() <= (size_t)max) {
        std::ostringstream os;
        for (size_t i = 0; i < ids.size(); i++) {
            if (i > 0) os << ", ";
            os << (i < names.size() ? names[i] : ids[i]);
        }
        os << " are typing...";
        return os.str();
    }
    std::ostringstream os;
    os << ids.size() << " people are typing...";
    return os.str();
}

std::string formatRoomName(const std::string& name, bool encrypted, bool direct) {
    std::string result = name;
    if (encrypted) result = "🔒 " + result;
    if (direct) result = "@" + result;
    return result;
}

std::string buildRoomSortKey(int64_t activity, bool fav, bool hl) {
    int prio = fav ? 0 : (hl ? 1 : 2);
    return std::to_string(prio) + ":" + std::to_string(activity);
}

RoomSummaryDisplay parseRoomSummary(const std::string& json) {
    RoomSummaryDisplay d;

    d.roomId         = parseJsonStringValue(json, "room_id");
    d.displayName    = parseJsonStringValue(json, "display_name");
    if (d.displayName.empty()) d.displayName = parseJsonStringValue(json, "displayName");
    d.topic          = parseJsonStringValue(json, "topic");
    d.avatarUrl      = parseJsonStringValue(json, "avatar_url");
    if (d.avatarUrl.empty()) d.avatarUrl = parseJsonStringValue(json, "avatarUrl");
    d.canonicalAlias = parseJsonStringValue(json, "canonical_alias");
    d.membership     = parseJsonStringValue(json, "membership");

    d.isDirect       = parseJsonBoolValue(json, "is_direct");
    d.isEncrypted    = parseJsonBoolValue(json, "is_encrypted");
    d.isFavourite    = parseJsonBoolValue(json, "is_favourite");
    d.isMuted        = parseJsonBoolValue(json, "is_muted");
    d.hasDraft       = parseJsonBoolValue(json, "has_draft");
    d.hasUnreadMessages = parseJsonBoolValue(json, "has_unread");

    d.notificationCount = static_cast<int>(parseJsonInt64Value(json, "notification_count"));
    d.highlightCount    = static_cast<int>(parseJsonInt64Value(json, "highlight_count"));
    d.threadNotificationCount = static_cast<int>(parseJsonInt64Value(json, "thread_notification_count"));
    d.joinedMembersCount      = static_cast<int>(parseJsonInt64Value(json, "joined_members_count"));
    d.invitedMembersCount     = static_cast<int>(parseJsonInt64Value(json, "invited_members_count"));

    d.lastMessage       = parseJsonStringValue(json, "last_message_body");
    d.lastMessageSender = parseJsonStringValue(json, "last_sender_name");
    d.lastMessageTs     = parseJsonInt64Value(json, "last_message_ts");
    d.formattedTime     = formatTimestamp(d.lastMessageTs);

    d.typingText = parseJsonStringValue(json, "typing_text");

    return d;
}

std::string formatRoomSummaryString(const RoomSummaryDisplay& s) {
    std::ostringstream os;
    os << s.displayName;
    if (!s.canonicalAlias.empty() && s.canonicalAlias != s.displayName) {
        os << " (" << s.canonicalAlias << ")";
    }
    if (s.isEncrypted) os << " [e2e]";
    if (s.isDirect) os << " [dm]";

    if (!s.lastMessage.empty()) {
        os << " — ";
        if (!s.lastMessageSender.empty()) {
            os << s.lastMessageSender << ": ";
        }
        os << s.lastMessage;
    }

    int unread = s.notificationCount;
    int hl = s.highlightCount;
    if (unread > 0) {
        os << " — ";
        if (hl > 0) os << hl << "@";
        os << unread << " unread";
    }

    return os.str();
}

std::string formatRoomDetailString(const RoomSummaryDisplay& s) {
    std::ostringstream os;
    os << "Room: " << s.displayName << "\n";
    os << "ID: " << s.roomId << "\n";
    if (!s.canonicalAlias.empty()) os << "Alias: " << s.canonicalAlias << "\n";
    if (!s.topic.empty()) os << "Topic: " << s.topic << "\n";
    os << "Membership: " << s.membership << "\n";
    os << "Joined members: " << s.joinedMembersCount << "\n";
    os << "Invited members: " << s.invitedMembersCount << "\n";
    os << "Encrypted: " << (s.isEncrypted ? "yes" : "no") << "\n";
    os << "Direct: " << (s.isDirect ? "yes" : "no") << "\n";
    os << "Unread notifications: " << s.notificationCount << "\n";
    os << "Highlights: " << s.highlightCount << "\n";
    if (!s.lastMessage.empty()) {
        os << "Last message: " << s.lastMessageSender << ": " << s.lastMessage << "\n";
        os << "Time: " << s.formattedTime << "\n";
    }
    return os.str();
}

} // namespace pcp
