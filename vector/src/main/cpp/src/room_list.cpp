#include "progressive/room_list.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::string assignRoomSection(const RoomListItem& room) {
    if (room.isInvited) return "Invites";
    if (room.isFavourite) return "Favourites";
    if (room.isLowPriority) return "Low Priority";
    if (room.isSpace) return "Spaces";
    if (room.isDirect) return "Directs";
    return "Rooms";
}

int computeRoomPriority(const RoomListItem& room) {
    int p = 0;
    if (room.isFavourite) p += 10000;
    if (room.isInvited) p += 9000;
    if (room.highlightCount > 0) p += 8000;
    if (room.hasUnread) p += 7000;
    if (room.isDirect) p += 1000;
    // Recent activity bonus (normalized to 0-999)
    if (room.lastActivityTs > 0) {
        p += static_cast<int>((room.lastActivityTs >> 20) & 0x3FF);
    }
    return p;
}

void sortRoomList(std::vector<RoomListItem>& rooms) {
    for (auto& r : rooms) r.priority = computeRoomPriority(r);
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.priority > b.priority;
    });
}

RoomListLayout computeRoomListLayout(const std::vector<RoomListItem>& rooms) {
    RoomListLayout layout;

    for (const auto& room : rooms) {
        auto section = assignRoomSection(room);
        if (section == "Favourites") layout.favourites.push_back(room);
        else if (section == "Directs") layout.directChats.push_back(room);
        else if (section == "Rooms") layout.rooms.push_back(room);
        else if (section == "Spaces") layout.spaces.push_back(room);
        else if (section == "Invites") layout.invites.push_back(room);
        else if (section == "Low Priority") layout.lowPriority.push_back(room);

        if (room.hasUnread) layout.totalUnread++;
        if (room.highlightCount > 0) layout.totalHighlights += room.highlightCount;
    }

    // Sort each section
    sortRoomList(layout.favourites);
    sortRoomList(layout.directChats);
    sortRoomList(layout.rooms);
    sortRoomList(layout.spaces);
    sortRoomList(layout.invites);

    return layout;
}

std::vector<RoomListItem> searchRoomList(const std::vector<RoomListItem>& rooms, const std::string& query) {
    if (query.empty()) return rooms;
    auto lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    std::vector<RoomListItem> result;
    for (const auto& room : rooms) {
        auto lowerName = room.name;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        if (lowerName.find(lowerQuery) != std::string::npos) {
            result.push_back(room);
        }
    }
    return result;
}

std::string getBadgeText(const RoomListItem& room, int maxDisplay) {
    if (room.highlightCount > 0) return "!";
    if (room.notificationCount > 0) {
        return room.notificationCount > maxDisplay ? std::to_string(maxDisplay) + "+"
                                                   : std::to_string(room.notificationCount);
    }
    return "";
}

std::string formatRoomListItem(const RoomListItem& room) {
    std::ostringstream out;
    if (room.isEncrypted) out << "🔒 ";
    out << room.name;
    if (!room.lastMessage.empty()) {
        out << "\n";
        if (!room.lastSender.empty()) out << room.lastSender << ": ";
        out << (room.lastMessage.size() > 50 ? room.lastMessage.substr(0, 47) + "..." : room.lastMessage);
    }
    auto badge = getBadgeText(room);
    if (!badge.empty()) out << "\n[" << badge << "]";
    return out.str();
}

std::string roomListLayoutToJson(const RoomListLayout& layout) {
    std::ostringstream json;
    json << "{";
    json << R"("totalUnread": )" << layout.totalUnread << ",";
    json << R"("totalHighlights": )" << layout.totalHighlights << ",";
    json << R"("favourites": )" << layout.favourites.size() << ",";
    json << R"("directChats": )" << layout.directChats.size() << ",";
    json << R"("rooms": )" << layout.rooms.size() << ",";
    json << R"("spaces": )" << layout.spaces.size() << ",";
    json << R"("invites": )" << layout.invites.size();
    json << "}";
    return json.str();
}

// ==== Notification State (Element Web algorithm) ====

NotificationState computeNotificationState(const RoomListItem& room) {
    NotificationState state;

    // Element Web logic: highlights override notifications
    if (room.highlightCount > 0) {
        state.level = NotificationLevel::RED;
        state.count = room.highlightCount;
    } else if (room.notificationCount > 0) {
        // Muted rooms get grey badge
        if (room.isMuted) {
            state.level = NotificationLevel::GREY;
        } else {
            state.level = NotificationLevel::RED;
        }
        state.count = room.notificationCount;
    } else {
        state.level = NotificationLevel::NONE;
        return state;
    }

    // Format badge text: "3", "99+"
    if (state.count > 99) state.badgeText = "99+";
    else if (state.count > 0) state.badgeText = std::to_string(state.count);

    state.showBadge = state.level != NotificationLevel::NONE;
    return state;
}

std::string notificationStateToJson(const NotificationState& state) {
    std::ostringstream json;
    json << "{";
    json << R"("level":")" << (state.level == NotificationLevel::RED ? "red" : 
                                state.level == NotificationLevel::GREY ? "grey" : "none") << R"(",)";
    json << R"("count":)" << state.count << ",";
    json << R"("badge_text":")" << state.badgeText << R"(",)";
    json << R"("show_badge":)" << (state.showBadge ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
