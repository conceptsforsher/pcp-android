#include "progressive/room_sort.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace progressive {

bool roomSortCompare(const RoomSortEntry& a, const RoomSortEntry& b) {
    // Original Kotlin (RoomComparator.kt):
    //   compareBy<RoomSummary>(
    //       { it.isFavourite.not() },     // favourites first
    //       { it.isDirect.not() },        // DMs before rooms
    //       { it.highlightCount == 0 },   // highlights first
    //       { it.notificationCount == 0 }, // notifications next
    //       { it.isServerNotice },        // server notices after normal
    //       { it.isSuggested },           // suggested at bottom
    //       { it.isLowPriority },         // low priority at bottom
    //       { it.lastEventTs * -1 }       // newest first
    //   )

    // 1. Favourites first
    bool aFav = a.tag == RoomTag::Favourite;
    bool bFav = b.tag == RoomTag::Favourite;
    if (aFav != bFav) return aFav;

    // 2. DMs before regular rooms
    if (a.isDirect != b.isDirect) return a.isDirect;

    // 3. Unread with highlights first
    bool aHigh = a.hasUnread && a.highlightCount > 0;
    bool bHigh = b.hasUnread && b.highlightCount > 0;
    if (aHigh != bHigh) return aHigh;

    // 4. Unread without highlights
    bool aUnread = a.hasUnread && a.notificationCount > 0;
    bool bUnread = b.hasUnread && b.notificationCount > 0;
    if (aUnread != bUnread) return aUnread;

    // 5. Manually marked unread
    if (a.isMarkedUnread != b.isMarkedUnread) return a.isMarkedUnread;

    // 6. Server notices below normal rooms
    bool aNotice = a.tag == RoomTag::ServerNotice;
    bool bNotice = b.tag == RoomTag::ServerNotice;
    if (aNotice != bNotice) return !aNotice;  // false (not notice) comes first

    // 7. Suggested rooms at bottom
    bool aSuggested = a.tag == RoomTag::Suggested;
    bool bSuggested = b.tag == RoomTag::Suggested;
    if (aSuggested != bSuggested) return !aSuggested;

    // 8. Low priority at very bottom
    bool aLow = a.tag == RoomTag::LowPriority;
    bool bLow = b.tag == RoomTag::LowPriority;
    if (aLow != bLow) return !aLow;

    // 9. Manual priority (higher = closer to top)
    if (a.priority != b.priority) return a.priority > b.priority;

    // 10. By last event timestamp — newest first
    // Original Kotlin: lastEventTs * -1 (negate for descending)
    if (a.lastEventTs != b.lastEventTs) return a.lastEventTs > b.lastEventTs;

    // 11. Alphabetical by display name as tiebreaker
    return a.displayName < b.displayName;
}

std::vector<RoomSortEntry> sortRooms(std::vector<RoomSortEntry> rooms) {
    std::sort(rooms.begin(), rooms.end(), roomSortCompare);
    return rooms;
}

int getRoomSortKey(const RoomSortEntry& room) {
    // Higher = closer to top. Each tier is 1000000 apart.
    int key = 0;

    // Favourites: +7M
    if (room.tag == RoomTag::Favourite) key += 7000000;

    // DMs: +6M
    if (room.isDirect) key += 6000000;

    // Unread highlights: +5M
    if (room.hasUnread && room.highlightCount > 0) key += 5000000;

    // Unread: +4M
    if (room.hasUnread && room.notificationCount > 0) key += 4000000;

    // Marked unread: +3M
    if (room.isMarkedUnread) key += 3000000;

    // Server notice penalty: max 1M
    if (room.tag == RoomTag::ServerNotice) key += 1000000;

    // Suggested: -1M penalty
    if (room.tag == RoomTag::Suggested) key -= 1000000;

    // Low priority: -2M penalty
    if (room.tag == RoomTag::LowPriority) key -= 2000000;

    // Priority: +10000 per level
    key += room.priority * 10000;

    // Timestamp: seconds since epoch / 60 (rough ordering)
    key += static_cast<int>((room.lastEventTs / 60000) & 0xFFFF);

    return key;
}

RoomTag parseRoomTag(const std::string& tagStr) {
    if (tagStr == "m.favourite") return RoomTag::Favourite;
    if (tagStr == "m.lowpriority") return RoomTag::LowPriority;
    if (tagStr == "m.server_notice") return RoomTag::ServerNotice;
    if (tagStr == "im.vector.suggested") return RoomTag::Suggested;
    return RoomTag::NoTag;
}

std::string roomTagToString(RoomTag tag) {
    switch (tag) {
        case RoomTag::Favourite: return "m.favourite";
        case RoomTag::LowPriority: return "m.lowpriority";
        case RoomTag::ServerNotice: return "m.server_notice";
        case RoomTag::Suggested: return "im.vector.suggested";
        default: return "";
    }
}

std::string getRoomSectionName(RoomTag tag, bool isDirect) {
    // Original Kotlin (RoomListViewModel.kt section headers)
    if (tag == RoomTag::Favourite) return "Favourites";
    if (isDirect) return "People";
    if (tag == RoomTag::LowPriority) return "Low Priority";
    if (tag == RoomTag::ServerNotice) return "System Alerts";
    if (tag == RoomTag::Suggested) return "Suggested";
    return "Rooms";
}

bool isDirectSection(const RoomSortEntry& room) {
    return room.isDirect && room.tag != RoomTag::LowPriority;
}

bool isFavouriteSection(const RoomSortEntry& room) {
    return room.tag == RoomTag::Favourite;
}

// ==== Breadcrumbs Sorting (from BreadcrumbsRoomComparator.kt:17-33) ====

bool breadcrumbsRoomCompare(const RoomSortEntry& a, const RoomSortEntry& b) {
    int aIdx = a.priority;  // breadcrumbs index stored in priority field
    int bIdx = b.priority;

    if (aIdx == NOT_IN_BREADCRUMBS) {
        if (bIdx == NOT_IN_BREADCRUMBS) {
            // Both not in breadcrumbs — fall back to chronological
            return a.lastEventTs > b.lastEventTs;
        }
        return false; // b has breadcrumbs, b comes first
    }
    if (bIdx == NOT_IN_BREADCRUMBS) {
        return true; // a has breadcrumbs, a comes first
    }
    // Both have breadcrumbs — sort by index (lower = more recent)
    return aIdx < bIdx;
}

std::vector<RoomSortEntry> sortRoomsByBreadcrumbs(std::vector<RoomSortEntry> rooms) {
    std::sort(rooms.begin(), rooms.end(), breadcrumbsRoomCompare);
    return rooms;
}

std::string roomSortEntryToJson(const RoomSortEntry& room) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(room.roomId) << R"(",)";
    json << R"("displayName": ")" << esc(room.displayName) << R"(",)";
    json << R"("lastEventTs": )" << room.lastEventTs << ",";
    json << R"("notificationCount": )" << room.notificationCount << ",";
    json << R"("highlightCount": )" << room.highlightCount << ",";
    json << R"("isDirect": )" << (room.isDirect ? "true" : "false") << ",";
    json << R"("hasUnread": )" << (room.hasUnread ? "true" : "false") << ",";
    json << R"("tag": ")" << esc(roomTagToString(room.tag)) << R"(",)";
    json << R"("isMarkedUnread": )" << (room.isMarkedUnread ? "true" : "false") << ",";
    json << R"("priority": )" << room.priority;
    json << "}";
    return json.str();
}

// ================================================================
// Extended Room Sort Functions
// ================================================================

int computeRoomSortKey(const RoomSortEntry& room, const RoomSortConfig& config) {
    // Original Kotlin: computeRoomSortKey() — integer key from config
    // Each criterion contributes to a multi-tier key
    int key = 0;
    int tierSize = 10000000;

    for (size_t i = 0; i < config.criteria.size(); ++i) {
        RoomSortCriteria crit = config.criteria[i];
        RoomSortOrder order = (i < config.orders.size()) ? config.orders[i] : RoomSortOrder::DESC;
        int tier = static_cast<int>(config.criteria.size() - i) * tierSize;

        bool ascending = (order == RoomSortOrder::ASC);

        switch (crit) {
            case RoomSortCriteria::NAME: {
                // Use first char of display name as rough sort
                int nameVal = room.displayName.empty() ? 0 : static_cast<int>(static_cast<unsigned char>(room.displayName[0]));
                key += ascending ? (tier - nameVal) : (tier + nameVal);
                break;
            }
            case RoomSortCriteria::ACTIVITY:
                key += ascending ? tier + static_cast<int>(room.lastEventTs / 60000) : tier - static_cast<int>(room.lastEventTs / 60000);
                break;
            case RoomSortCriteria::UNREAD:
                key += ascending ? (room.hasUnread ? tier + 1 : tier) : (room.hasUnread ? tier + 1 : tier + 0);
                break;
            case RoomSortCriteria::NOTIFICATIONS:
                key += ascending ? tier + room.notificationCount : tier + (10000 - room.notificationCount);
                break;
            case RoomSortCriteria::FAVOURITE: {
                bool isFav = (room.tag == RoomTag::Favourite);
                key += ascending ? (isFav ? tier + 1 : tier) : (isFav ? tier + 1 : tier + 0);
                break;
            }
            case RoomSortCriteria::DIRECT_CHAT:
                key += ascending ? (room.isDirect ? tier + 1 : tier) : (room.isDirect ? tier + 1 : tier + 0);
                break;
            case RoomSortCriteria::SPACE:
                // Space grouping is handled separately via groupRoomsForDisplay
                break;
            case RoomSortCriteria::TAG: {
                int tagVal = static_cast<int>(room.tag);
                key += ascending ? tier + tagVal : tier + (10 - tagVal);
                break;
            }
            case RoomSortCriteria::MANUAL:
                key += ascending ? tier + room.priority : tier + (10000 - room.priority);
                break;
        }
    }

    return key;
}

RoomSortConfig getDefaultSortConfig() {
    // Original Kotlin: getDefaultSortConfig() — RECENT preset
    return getPresetSortConfig(RoomSortPreset::RECENT);
}

RoomSortConfig getPresetSortConfig(RoomSortPreset preset) {
    // Original Kotlin: getPresetSortConfig()
    RoomSortConfig config;

    switch (preset) {
        case RoomSortPreset::RECENT:
            config.criteria = {
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::NOTIFICATIONS,
                RoomSortCriteria::ACTIVITY
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::DESC,
                RoomSortOrder::DESC
            };
            config.pinFavourites = true;
            config.groupBySpace = false;
            config.showInvitesFirst = true;
            break;

        case RoomSortPreset::UNREAD_FIRST:
            config.criteria = {
                RoomSortCriteria::NOTIFICATIONS,
                RoomSortCriteria::UNREAD,
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::ACTIVITY
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::DESC,
                RoomSortOrder::DESC,
                RoomSortOrder::DESC
            };
            config.pinFavourites = true;
            config.showInvitesFirst = true;
            break;

        case RoomSortPreset::A_TO_Z:
            config.criteria = {
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::NAME
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::ASC
            };
            config.pinFavourites = true;
            config.showInvitesFirst = false;
            break;

        case RoomSortPreset::Z_TO_A:
            config.criteria = {
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::NAME
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::DESC
            };
            config.pinFavourites = true;
            config.showInvitesFirst = false;
            break;

        case RoomSortPreset::FAVOURITES_FIRST:
            config.criteria = {
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::ACTIVITY
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::DESC
            };
            config.pinFavourites = true;
            config.showInvitesFirst = true;
            break;

        case RoomSortPreset::CUSTOM:
        default:
            config.criteria = {
                RoomSortCriteria::FAVOURITE,
                RoomSortCriteria::NOTIFICATIONS,
                RoomSortCriteria::ACTIVITY
            };
            config.orders = {
                RoomSortOrder::DESC,
                RoomSortOrder::DESC,
                RoomSortOrder::DESC
            };
            config.pinFavourites = true;
            config.showInvitesFirst = true;
            break;
    }

    return config;
}

std::vector<RoomSortEntry> sortRoomsByConfig(std::vector<RoomSortEntry> rooms, const RoomSortConfig& config) {
    // Original Kotlin: sortRoomsByConfig() — multi-criteria sort
    std::sort(rooms.begin(), rooms.end(),
        [&config](const RoomSortEntry& a, const RoomSortEntry& b) -> bool {
            int keyA = computeRoomSortKey(a, config);
            int keyB = computeRoomSortKey(b, config);
            if (keyA != keyB) return keyA > keyB;  // higher key = closer to top

            // Tiebreaker: most recent first
            if (a.lastEventTs != b.lastEventTs) return a.lastEventTs > b.lastEventTs;

            // Alphabetical as final tiebreaker
            return a.displayName < b.displayName;
        });
    return rooms;
}

std::vector<RoomGroupInfo> groupRoomsForDisplay(const std::vector<RoomSortEntry>& rooms,
                                                  const RoomGroupingConfig& config) {
    // Original Kotlin: groupRoomsForDisplay() — segments rooms into groups
    std::vector<RoomGroupInfo> groups;

    // Group by tag
    std::unordered_map<RoomTag, std::vector<RoomSortEntry>> byTag;
    for (const auto& r : rooms) {
        byTag[r.tag].push_back(r);
    }

    // Process in display order: Favourite, untagged, LowPriority
    auto addGroup = [&](RoomTag tag, const std::string& name) {
        auto it = byTag.find(tag);
        if (it == byTag.end()) return;
        if (config.collapseEmpty && it->second.empty()) return;

        RoomGroupInfo group = computeRoomGroupSummary(roomTagToString(tag), name, it->second, config);
        groups.push_back(group);
    };

    if (config.groupByTag) {
        addGroup(RoomTag::Favourite, "Favourites");
        addGroup(RoomTag::NoTag, "Rooms");
        addGroup(RoomTag::LowPriority, "Low Priority");
        addGroup(RoomTag::ServerNotice, "System Alerts");
        addGroup(RoomTag::Suggested, "Suggested");
    } else {
        // Single flat group
        RoomGroupInfo group = computeRoomGroupSummary("all", "All Rooms", rooms, config);
        groups.push_back(group);
    }

    return groups;
}

RoomGroupInfo computeRoomGroupSummary(const std::string& groupId, const std::string& groupName,
                                        const std::vector<RoomSortEntry>& rooms,
                                        const RoomGroupingConfig& config) {
    // Original Kotlin: computeRoomGroupSummary()
    RoomGroupInfo info;
    info.groupId = groupId;
    info.groupName = groupName;
    info.isCollapsible = config.groupByTag;

    for (const auto& r : rooms) {
        info.rooms.push_back(r.roomId);
        if (r.hasUnread) info.unreadCount++;
        info.highlightCount += r.highlightCount;
    }

    info.isCollapsed = config.collapseEmpty && rooms.empty();
    return info;
}

} // namespace progressive
