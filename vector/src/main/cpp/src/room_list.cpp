#include "progressive/room_list.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>

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

// ==== Sort Room List by Mode ====
//
// Original Kotlin (RoomListManager.kt sort by different modes):
//   ACTIVITY: sort by lastActivityTs descending (most recent first)
//   ALPHABETICAL: sort by name ascending, case-insensitive
//   PRIORITY: sort by composite priority (default)
//   MANUAL: no-op (user-defined order not implemented)

void sortRoomList(std::vector<RoomListItem>& rooms, RoomSortMode mode) {
    switch (mode) {
        case RoomSortMode::ACTIVITY:
            std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
                return a.lastActivityTs > b.lastActivityTs;
            });
            break;

        case RoomSortMode::ALPHABETICAL: {
            std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
                std::string na = a.name;
                std::string nb = b.name;
                std::transform(na.begin(), na.end(), na.begin(), ::tolower);
                std::transform(nb.begin(), nb.end(), nb.begin(), ::tolower);
                return na < nb;
            });
            break;
        }

        case RoomSortMode::PRIORITY:
            sortRoomList(rooms);
            break;

        case RoomSortMode::MANUAL:
            break;
    }
}

// ==== Group Room List ====
//
// Original Kotlin (RoomListManager.kt groupRooms):
//   Group rooms into sections ordered: Favourites -> Directs -> Rooms -> Spaces -> Invites -> Low Priority
//   Each section gets a title, sorted rooms, and metadata

std::vector<RoomListSection> groupRoomList(const std::vector<RoomListItem>& rooms) {
    std::vector<RoomListItem> favs, directs, normal, spaces, invites, lowPrio;
    for (const auto& room : rooms) {
        auto sec = assignRoomSection(room);
        if (sec == "Favourites") favs.push_back(room);
        else if (sec == "Directs") directs.push_back(room);
        else if (sec == "Rooms") normal.push_back(room);
        else if (sec == "Spaces") spaces.push_back(room);
        else if (sec == "Invites") invites.push_back(room);
        else if (sec == "Low Priority") lowPrio.push_back(room);
    }

    sortRoomList(favs);
    sortRoomList(directs);
    sortRoomList(normal);
    sortRoomList(spaces);
    sortRoomList(invites);
    sortRoomList(lowPrio);

    auto buildSection = [](const std::string& title, const std::string& category,
                           std::vector<RoomListItem>& roomVec) -> RoomListSection {
        RoomListSection section;
        section.title = title;
        section.category = category;
        section.rooms = std::move(roomVec);
        section.isEmpty = section.rooms.empty();
        for (const auto& r : section.rooms) {
            if (r.hasUnread) section.unreadCount++;
            section.highlightCount += r.highlightCount;
        }
        return section;
    };

    std::vector<RoomListSection> sections;
    sections.push_back(buildSection("Favourites", "favourites", favs));
    sections.push_back(buildSection("People", "directs", directs));
    sections.push_back(buildSection("Rooms", "rooms", normal));
    sections.push_back(buildSection("Spaces", "spaces", spaces));
    sections.push_back(buildSection("Invites", "invites", invites));
    sections.push_back(buildSection("Low Priority", "low_priority", lowPrio));

    return sections;
}

// ==== Build Room List Snapshot ====
//
// Original Kotlin (RoomListManager.kt buildSnapshot):
//   Build headers for grouped display. Optionally filter by query.
//   Returns a list of RoomGroupHeader for each non-empty section.

std::vector<RoomGroupHeader> buildRoomListSnapshot(
    const std::vector<RoomListItem>& rooms,
    const std::string& filterQuery)
{
    std::vector<RoomListItem> filtered;
    if (!filterQuery.empty()) {
        filtered = searchRoomList(rooms, filterQuery);
    } else {
        filtered = rooms;
    }

    auto sections = groupRoomList(filtered);

    std::vector<RoomGroupHeader> headers;
    int priority = 0;
    for (const auto& section : sections) {
        RoomGroupHeader header;
        header.category = section.category;
        header.title = section.title;
        header.count = static_cast<int>(section.rooms.size());
        header.unreadCount = section.unreadCount;
        header.priority = priority++;

        if (section.category == "invites") {
            header.isCollapsible = false;
            header.isExpanded = true;
        }

        if (header.count > 0 || section.category == "invites") {
            headers.push_back(header);
        }
    }

    return headers;
}

// ==== Get Room List Section ====
//
// Original Kotlin (RoomListManager.kt categorizeRoom):
//   Return a header describing which section a room belongs to

RoomGroupHeader getRoomListSection(const RoomListItem& room) {
    RoomGroupHeader header;
    std::string sec = assignRoomSection(room);

    if (sec == "Favourites") {
        header.category = "favourites"; header.title = "Favourites"; header.priority = 0;
    } else if (sec == "Directs") {
        header.category = "directs"; header.title = "People"; header.priority = 1;
    } else if (sec == "Rooms") {
        header.category = "rooms"; header.title = "Rooms"; header.priority = 2;
    } else if (sec == "Spaces") {
        header.category = "spaces"; header.title = "Spaces"; header.priority = 3;
    } else if (sec == "Invites") {
        header.category = "invites"; header.title = "Invites";
        header.isCollapsible = false; header.priority = 4;
    } else if (sec == "Low Priority") {
        header.category = "low_priority"; header.title = "Low Priority"; header.priority = 5;
    }

    return header;
}

// ==== Check Space Membership ====
//
// Original Kotlin (RoomListManager.kt isInMultipleSpaces):
//   Uses spaceChildren map: spaceId -> [childRoomIds]
//   Returns true if the room appears as a child in more than one space

bool isRoomInMultipleSpaces(
    const std::string& roomId,
    const std::vector<std::pair<std::string, std::vector<std::string>>>& spaceChildren)
{
    int spaceCount = 0;
    for (const auto& [spaceId, children] : spaceChildren) {
        for (const auto& childId : children) {
            if (childId == roomId) {
                spaceCount++;
                if (spaceCount > 1) return true;
                break;
            }
        }
    }
    return false;
}

// ==== Format Room Last Activity ====
//
// Original Kotlin (RoomSummaryFormatter.kt formatLastActivity):
//   Converts a UTC epoch-millis timestamp to a relative time string.
//   Logic ported from Element Android's DateFormatter:
//     < 1 min -> "now", < 60 min -> "Xm ago", < 24 hrs -> "Xh ago"
//     Yesterday -> "Yesterday", < 7 days -> weekday name
//     This year -> "Jan 5", Older -> "Jan 5, 2020"

std::string formatRoomLastActivity(int64_t lastActivityTs, int64_t nowMs) {
    if (lastActivityTs <= 0) return "";

    if (nowMs == 0) {
        nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    int64_t diffMs = nowMs - lastActivityTs;
    if (diffMs < 0) diffMs = 0;

    int64_t diffSec = diffMs / 1000;
    int64_t diffMin = diffSec / 60;
    int64_t diffHr = diffMin / 60;
    int64_t diffDay = diffHr / 24;

    if (diffMin < 1) return "now";
    if (diffMin < 60) return std::to_string(diffMin) + "m ago";
    if (diffHr < 24) return std::to_string(diffHr) + "h ago";

    auto dur = std::chrono::milliseconds(lastActivityTs);
    auto tp = std::chrono::system_clock::time_point(dur);
    std::time_t eventTime = std::chrono::system_clock::to_time_t(tp);
    std::time_t nowTime = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());

    std::tm eventTm = *std::localtime(&eventTime);
    std::tm nowTm = *std::localtime(&nowTime);

    if (eventTm.tm_year == nowTm.tm_year &&
        eventTm.tm_mon == nowTm.tm_mon &&
        eventTm.tm_mday == nowTm.tm_mday) {
        return std::to_string(diffHr) + "h ago";
    }

    int eventDayOfYear = eventTm.tm_yday;
    int nowDayOfYear = nowTm.tm_yday;
    if (eventTm.tm_year != nowTm.tm_year) {
        int daysInEventYear = (eventTm.tm_year % 4 == 0 && eventTm.tm_year % 100 != 0)
                              || eventTm.tm_year % 400 == 0 ? 366 : 365;
        eventDayOfYear = eventTm.tm_yday - daysInEventYear;
    }

    if (nowDayOfYear - eventDayOfYear == 1) return "Yesterday";

    if (diffDay < 7) {
        static const char* weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        return weekdays[eventTm.tm_wday];
    }

    if (eventTm.tm_year == nowTm.tm_year) {
        static const char* months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        return std::string(months[eventTm.tm_mon]) + " " + std::to_string(eventTm.tm_mday);
    }

    {
        static const char* months[] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        return std::string(months[eventTm.tm_mon]) + " " +
               std::to_string(eventTm.tm_mday) + ", " +
               std::to_string(eventTm.tm_year + 1900);
    }
}

// ==== JSON Serialization for Section/Header ====
//
// Original Kotlin: Serialize room list structures for JNI bridge to Kotlin UI

std::string roomListSectionToJson(const RoomListSection& section) {
    std::ostringstream json;
    json << "{";
    json << R"("title":")" << section.title << R"(",)";
    json << R"("category":")" << section.category << R"(",)";
    json << R"("is_collapsed":)" << (section.isCollapsed ? "true" : "false") << ",";
    json << R"("unread_count":)" << section.unreadCount << ",";
    json << R"("highlight_count":)" << section.highlightCount << ",";
    json << R"("is_empty":)" << (section.isEmpty ? "true" : "false") << ",";
    json << R"("room_count":)" << section.rooms.size() << ",";

    json << R"("room_ids":[)";
    bool first = true;
    for (const auto& room : section.rooms) {
        if (!first) json << ",";
        first = false;
        json << R"(")" << room.roomId << R"(")";
    }
    json << "]";

    json << "}";
    return json.str();
}

std::string roomGroupHeaderToJson(const RoomGroupHeader& header) {
    std::ostringstream json;
    json << "{";
    json << R"("category":")" << header.category << R"(",)";
    json << R"("title":")" << header.title << R"(",)";
    json << R"("count":)" << header.count << ",";
    json << R"("is_collapsible":)" << (header.isCollapsible ? "true" : "false") << ",";
    json << R"("is_expanded":)" << (header.isExpanded ? "true" : "false") << ",";
    json << R"("unread_count":)" << header.unreadCount << ",";
    json << R"("is_sticky":)" << (header.isSticky ? "true" : "false") << ",";
    json << R"("priority":)" << header.priority;
    json << "}";
    return json.str();
}

std::string roomListSnapshotToJson(const std::vector<RoomListSection>& sections) {
    std::ostringstream json;
    json << "[";
    bool firstSection = true;
    for (const auto& section : sections) {
        if (!firstSection) json << ",";
        firstSection = false;
        json << roomListSectionToJson(section);
    }
    json << "]";
    return json.str();
}

} // namespace progressive
