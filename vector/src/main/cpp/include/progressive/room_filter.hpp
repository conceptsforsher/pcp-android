#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ==== Room List Filtering & Processing (from Element X) ====
//
// Ported from Element X's RoomListFilterMapper, RoomSummaryListProcessor,
// and RoomListRoomSummaryFactory.
//
// These algorithms filter, sort, and deduplicate room lists as they
// arrive from sliding sync or /sync responses.

// Filter categories (from Element X RoomListFilter)
enum class RoomFilterCategory {
    ALL = 0,
    PEOPLE = 1,      // Direct messages only
    GROUPS = 2,       // Group rooms (not DM, not space)
    SPACES = 3,       // Space rooms only
    FAVOURITES = 4,   // Starred/favourite rooms
    UNREADS = 5,      // Rooms with unread messages
    INVITES = 6       // Pending invitations
};

// Room list filter parameters
struct RoomListFilter {
    RoomFilterCategory category = RoomFilterCategory::ALL;
    std::string searchQuery;         // Filter by room name (case-insensitive)
    bool showFavourites = false;     // Favourites-only toggle
    bool hideLeft = true;            // Hide rooms where user left/declined
    bool hideInvites = false;        // Hide pending invites

    bool isEmpty() const {
        return category == RoomFilterCategory::ALL
            && searchQuery.empty()
            && !showFavourites;
    }
};

// Room entry for filtering
struct RoomFilterEntry {
    std::string roomId;
    std::string displayName;         // Computed display name
    std::string canonicalAlias;
    bool isDirect = false;
    bool isSpace = false;
    bool isFavourite = false;
    bool isInvite = false;
    bool hasLeft = false;            // membership = leave/ban
    int notificationCount = 0;
    int highlightCount = 0;
    int64_t lastActivityMs = 0;      // Latest event timestamp
    bool isEncrypted = false;
    int joinedMembers = 0;
};

// Filter and sort room entries.
// Returns filtered + sorted list in display order.
//
// Original Element X:
//   RoomListFilterMapper (base filters: NonSpace && NonLeft, or Space && Invite)
//   RoomSummaryListProcessor (Append, Remove, Clear, PushFront, PushBack, Reset)
//   Sorting: by priority (favourites first), then by recency

inline std::vector<RoomFilterEntry> filterAndSortRooms(
    const std::vector<RoomFilterEntry>& input,
    const RoomListFilter& filter)
{
    // Step 1: Apply filters
    std::vector<RoomFilterEntry> result;

    for (const auto& room : input) {
        // Hide left rooms unless explicitly shown
        if (room.hasLeft && filter.hideLeft) continue;

        // Hide invites unless category is INVITES
        if (room.isInvite && !filter.hideInvites && filter.category != RoomFilterCategory::INVITES) continue;

        // Category filter
        switch (filter.category) {
            case RoomFilterCategory::PEOPLE:
                if (!room.isDirect || room.isSpace) continue;
                break;
            case RoomFilterCategory::GROUPS:
                if (room.isDirect || room.isSpace) continue;
                break;
            case RoomFilterCategory::SPACES:
                if (!room.isSpace) continue;
                break;
            case RoomFilterCategory::FAVOURITES:
                if (!room.isFavourite) continue;
                break;
            case RoomFilterCategory::UNREADS:
                if (room.notificationCount == 0 && room.highlightCount == 0) continue;
                break;
            case RoomFilterCategory::INVITES:
                if (!room.isInvite) continue;
                break;
            case RoomFilterCategory::ALL:
            default:
                break;
        }

        // Search query filter (case-insensitive substring match)
        if (!filter.searchQuery.empty()) {
            std::string nameLower = room.displayName;
            std::string aliasLower = room.canonicalAlias;
            std::string queryLower = filter.searchQuery;
            for (char& c : nameLower) c = tolower(c);
            for (char& c : aliasLower) c = tolower(c);
            for (char& c : queryLower) c = tolower(c);

            if (nameLower.find(queryLower) == std::string::npos &&
                aliasLower.find(queryLower) == std::string::npos) {
                continue;
            }
        }

        // Show favourites only
        if (filter.showFavourites && !room.isFavourite) continue;

        result.push_back(room);
    }

    // Step 2: Sort by priority then recency
    // Original Element X: favourites first, then by last activity descending
    std::sort(result.begin(), result.end(),
        [](const RoomFilterEntry& a, const RoomFilterEntry& b) {
            // Favourites always first
            if (a.isFavourite != b.isFavourite) return a.isFavourite > b.isFavourite;
            // Invites after favourites but before regular rooms (sorted by arrival)
            if (a.isInvite != b.isInvite) return b.isInvite > a.isInvite;
            // Spaces before regular rooms
            if (a.isSpace != b.isSpace) return a.isSpace > b.isSpace;
            // Direct messages before group rooms
            if (a.isDirect != b.isDirect) return a.isDirect > b.isDirect;
            // Unread rooms before read rooms
            bool aUnread = a.notificationCount > 0 || a.highlightCount > 0;
            bool bUnread = b.notificationCount > 0 || b.highlightCount > 0;
            if (aUnread != bUnread) return aUnread > bUnread;
            // Finally, by last activity (most recent first)
            return a.lastActivityMs > b.lastActivityMs;
        });

    return result;
}

// ==== Room Version Deduplication ====
//
// Original Element X: DeduplicateVersions filter
// When a room is upgraded, there are two entries: the old room (tombstoned)
// and the new room (active). The old room should be hidden.

// Check if a room is a predecessor (upgraded/tombstoned).
// Predecessor rooms have a replacement_room field in their tombstone event.

struct RoomUpgradeInfo {
    std::string roomId;
    std::string predecessorRoomId;   // If this is a new room, the old one it replaces
    std::string replacementRoomId;   // If tombstoned, the new room to go to
    bool isPredecessor = false;      // This room has been upgraded away from
};

// Deduplicate room versions: keep only the latest version.
// For each predecessor → successor pair, remove the predecessor.
inline std::vector<RoomFilterEntry> deduplicateRoomVersions(
    const std::vector<RoomFilterEntry>& rooms,
    const std::vector<RoomUpgradeInfo>& versionInfo)
{
    // Build map: predecessorRoomId → successor roomId
    std::unordered_map<std::string, std::string> predecessors;
    for (const auto& v : versionInfo) {
        if (v.isPredecessor && !v.replacementRoomId.empty()) {
            predecessors[v.roomId] = v.replacementRoomId;
        }
    }

    // Filter: if a room is a predecessor AND its successor is also in the list, skip it
    std::vector<RoomFilterEntry> result;
    for (const auto& room : rooms) {
        auto it = predecessors.find(room.roomId);
        if (it != predecessors.end()) {
            // Check if the successor is also in the list
            bool successorPresent = false;
            for (const auto& r : rooms) {
                if (r.roomId == it->second) { successorPresent = true; break; }
            }
            if (successorPresent) continue; // Skip predecessor
        }
        result.push_back(room);
    }

    return result;
}

} // namespace progressive
