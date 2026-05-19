#include "progressive/room_summary.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

RoomListStats computeRoomListStats(const std::vector<RoomSummaryInfo>& rooms) {
    RoomListStats stats;
    for (const auto& room : rooms) {
        stats.totalRooms++;
        if (room.hasUnread) stats.totalUnread++;
        if (room.highlightCount > 0 || room.hasMention) stats.totalHighlights += room.highlightCount;
        if (room.isDirect) stats.totalDirectChats++;
        if (!room.isDirect && !room.isSpace) stats.totalGroupRooms++;
        if (room.isSpace) stats.totalSpaces++;
        if (room.isInvited) stats.totalInvites++;
        if (room.isFavourite) stats.totalFavourites++;
    }
    return stats;
}

int computeRoomPriority(const RoomSummaryInfo& room) {
    int priority = 0;

    // Favourites always first
    if (room.isFavourite) priority += 1000;

    // Invites are important
    if (room.isInvited) priority += 900;

    // Pings/highlights
    if (room.highlightCount > 0 || room.hasMention) priority += 800;

    // Unread messages
    if (room.hasUnread) priority += 700;

    // Direct chats slightly above group rooms
    if (room.isDirect) priority += 100;

    // Add small timestamp component (normalized)
    if (room.lastActivityTs > 0) {
        priority += static_cast<int>((room.lastActivityTs >> 20) & 0xFF);
    }

    return priority;
}

void sortRoomsByPriority(std::vector<RoomSummaryInfo>& rooms) {
    for (auto& room : rooms) {
        room.sortPriority = computeRoomPriority(room);
    }
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.sortPriority > b.sortPriority;
    });
}

void sortRoomsByActivity(std::vector<RoomSummaryInfo>& rooms) {
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.lastActivityTs > b.lastActivityTs;
    });
}

void sortRoomsByName(std::vector<RoomSummaryInfo>& rooms) {
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.displayName < b.displayName;
    });
}

bool needsAttention(const RoomSummaryInfo& room) {
    return room.isInvited || room.hasMention || room.highlightCount > 0;
}

std::string getNotificationBadge(const RoomSummaryInfo& room) {
    if (room.highlightCount > 0 || room.hasMention) {
        return "!";
    }
    if (room.notificationCount > 0) {
        return room.notificationCount > 99 ? "99+" : std::to_string(room.notificationCount);
    }
    return "";
}

std::string formatLastMessagePreview(const std::string& sender, const std::string& body, bool isEncrypted) {
    std::ostringstream out;
    if (!sender.empty()) out << sender << ": ";
    if (isEncrypted && body.empty()) out << "[Encrypted message]";
    else if (body.empty()) out << "[No preview]";
    else if (body.size() > 80) out << body.substr(0, 77) << "...";
    else out << body;
    return out.str();
}

std::string formatRoomPreview(const RoomSummaryInfo& room, bool showSender) {
    std::ostringstream out;

    // Encryption indicator
    if (room.isEncrypted) out << "🔒 ";

    // Room name
    out << room.displayName;

    // Last message
    if (!room.lastMessageBody.empty()) {
        out << "\n";
        if (showSender && !room.lastMessageSender.empty()) {
            out << room.lastMessageSender << ": ";
        }
        out << (room.lastMessageBody.size() > 60
            ? room.lastMessageBody.substr(0, 57) + "..."
            : room.lastMessageBody);
    }

    return out.str();
}

std::string roomListStatsToJson(const RoomListStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("totalRooms": )" << stats.totalRooms << ",";
    json << R"("totalUnread": )" << stats.totalUnread << ",";
    json << R"("totalHighlights": )" << stats.totalHighlights << ",";
    json << R"("totalDirectChats": )" << stats.totalDirectChats << ",";
    json << R"("totalGroupRooms": )" << stats.totalGroupRooms << ",";
    json << R"("totalSpaces": )" << stats.totalSpaces << ",";
    json << R"("totalInvites": )" << stats.totalInvites << ",";
    json << R"("totalFavourites": )" << stats.totalFavourites;
    json << "}";
    return json.str();
}

} // namespace progressive
