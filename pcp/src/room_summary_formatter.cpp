#include "pcp/room_summary_formatter.hpp"
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
    // Favourites first, then highlights, then by recency
    int prio = fav ? 0 : (hl ? 1 : 2);
    return std::to_string(prio) + ":" + std::to_string(activity);
}

} // namespace pcp
