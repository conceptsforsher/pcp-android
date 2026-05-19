#include "progressive/room_sort.hpp"
#include <sstream>
#include <algorithm>

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

} // namespace progressive
