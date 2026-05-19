#ifndef PROGRESSIVE_ROOM_SORT_HPP
#define PROGRESSIVE_ROOM_SORT_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room Sorting / Ordering ----
// Ported from: im.vector.app.features.home.RoomListViewModel.kt (sorting logic)
//              org.matrix.android.sdk.api.session.room.model.RoomTag.kt
//              im.vector.app.features.home.RoomComparator.kt

// Matrix room tags (from RoomTag.kt)
enum class RoomTag {
    NoTag,              // untagged (default)
    Favourite,          // m.favourite — pinned to top
    LowPriority,        // m.lowpriority — below normal
    ServerNotice,       // m.server_notice — system messages
    Suggested,          // im.vector.suggested — suggested rooms
};

struct RoomSortEntry {
    std::string roomId;
    std::string displayName;
    int64_t lastEventTs = 0;       // epoch ms — most recent event
    int notificationCount = 0;     // unread notifications
    int highlightCount = 0;        // @mentions
    bool isDirect = false;         // DM room
    bool hasUnread = false;        // has any unread messages
    RoomTag tag = RoomTag::NoTag;  // user-defined tag
    bool isMarkedUnread = false;   // manually marked unread
    int priority = 0;              // manual priority (0 = default)
};

// Compare function: returns true if 'a' should come before 'b'.
// Ordering rules (original Kotlin RoomComparator.kt):
//   1. Favourites first
//   2. DMs above non-DMs
//   3. Unread (with highlights) above unread (without highlights) above read
//   4. Server notices above normal
//   5. Suggested rooms below everything
//   6. Low priority below everything
//   7. By last event timestamp (newest first) within same group
bool roomSortCompare(const RoomSortEntry& a, const RoomSortEntry& b);

// Sort a vector of rooms using the compare function.
std::vector<RoomSortEntry> sortRooms(std::vector<RoomSortEntry> rooms);

// Get a sort key for a room (for efficient sorting in SQL/queries).
// Returns an integer where higher = closer to top.
int getRoomSortKey(const RoomSortEntry& room);

// Parse a Matrix room tag string to RoomTag enum.
// "m.favourite" → Favourite, "m.lowpriority" → LowPriority, etc.
RoomTag parseRoomTag(const std::string& tagStr);

// Convert RoomTag to string representation.
std::string roomTagToString(RoomTag tag);

// Get a human-readable section header for a group of rooms.
// "Favourites", "Direct Messages", "Rooms", "Low Priority"
std::string getRoomSectionName(RoomTag tag, bool isDirect);

// Check if a room should be in the "People" (DM) section.
bool isDirectSection(const RoomSortEntry& room);

// Check if a room should be in the "Favourites" section.
bool isFavouriteSection(const RoomSortEntry& room);

// Room category filter — from RoomCategoryFilter.kt (38L)
enum class RoomCategory { All, OnlyDm, OnlyRooms, OnlyWithNotifications }; 

// ---- Breadcrumbs Sorting (from BreadcrumbsRoomComparator.kt 35L) ----
// Breadcrumbs are recently visited rooms. They appear at the top of the room list.
// Rooms with breadcrumbsIndex != -1 are sorted by index (lower = more recent).
// Rooms without breadcrumbs fall back to chronological order.

constexpr int NOT_IN_BREADCRUMBS = -1;

// Sort rooms by breadcrumbs first, then chronologically.
// Original: leftBreadcrumbsIndex <=> rightBreadcrumbsIndex, fallback to timestamp
std::vector<RoomSortEntry> sortRoomsByBreadcrumbs(std::vector<RoomSortEntry> rooms);

// Compare two rooms by breadcrumbs index.
bool breadcrumbsRoomCompare(const RoomSortEntry& a, const RoomSortEntry& b);

// Format sort results as JSON for the Kotlin UI.
std::string roomSortEntryToJson(const RoomSortEntry& room);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_SORT_HPP
