#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

namespace progressive {

// ==== Room Sort Mode ====
//
// Original Kotlin (RoomListManager.kt sort modes):
//   Different ways to sort the room list
enum class RoomSortMode {
    ACTIVITY,       // Sort by last activity timestamp (most recent first)
    ALPHABETICAL,   // Sort by room name alphabetically
    PRIORITY,       // Sort by composite priority score
    MANUAL          // User-defined order (dragged, not implemented here)
};

// ---- Room List Sorting & Filtering ----

struct RoomListItem {
    std::string roomId;
    std::string name;              // display name
    std::string avatarUrl;
    std::string lastMessage;       // preview text
    std::string lastSender;
    int64_t lastActivityTs = 0;    // epoch ms
    int notificationCount = 0;
    int highlightCount = 0;
    bool isDirect = false;
    bool isInvited = false;
    bool isFavourite = false;
    bool isLowPriority = false;
    bool isSpace = false;
    bool hasUnread = false;
    bool hasDraft = false;
    bool isEncrypted = false;
    bool isMuted = false;          // notifications disabled
    int memberCount = 0;

    // Computed priority (higher = more important)
    int priority = 0;

    // Section for grouped display
    std::string section;           // "Favourites", "Directs", "Rooms", "Spaces", "Invites", "Low Priority"

    // Original Kotlin: additional metadata for room list display
    std::string canonicalAlias;
    std::string topic;
    std::string joinRule;          // "public", "invite", etc.
    bool isTombstoned = false;
    bool hasUnsentMessages = false;
    int64_t highlightTimestamp = 0; // timestamp of most recent highlight
};

// ==== RoomListSection ====
//
// Original Kotlin (RoomListManager.kt RoomListSection):
//   Represents one collapsible section in the room list UI
struct RoomListSection {
    std::string title;              // Section header text
    std::string category;           // Category key: "favourites", "directs", "rooms", "spaces", "invites", "low_priority"
    std::vector<RoomListItem> rooms; // Rooms in this section
    bool isCollapsed = false;       // UI state: is this section collapsed
    int unreadCount = 0;            // Total unread in this section
    int highlightCount = 0;         // Total highlights in this section
    bool isEmpty = true;            // True if no rooms in this section (after filtering)

    // Original Kotlin: get display info
    bool hasUnreadActivity() const { return unreadCount > 0 || highlightCount > 0; }
};

// ==== RoomGroupHeader ====
//
// Original Kotlin (RoomListManager.kt room group headers):
//   Display header for each room category group
struct RoomGroupHeader {
    std::string category;           // Category key
    std::string title;              // Display title for the header
    int count = 0;                  // Number of rooms in this group
    bool isCollapsible = true;      // Can this header be tapped to collapse
    bool isExpanded = true;         // Current expansion state
    int unreadCount = 0;            // Total unread notifications in group
    bool isSticky = true;           // Should this header remain visible when scrolling
    int priority = 0;               // Display ordering priority
};

struct RoomListLayout {
    std::vector<RoomListItem> favourites;
    std::vector<RoomListItem> directChats;
    std::vector<RoomListItem> rooms;
    std::vector<RoomListItem> spaces;
    std::vector<RoomListItem> invites;
    std::vector<RoomListItem> lowPriority;
    int totalUnread = 0;
    int totalHighlights = 0;
};

// Compute room list layout with sections.
RoomListLayout computeRoomListLayout(const std::vector<RoomListItem>& rooms);

// Assign each room to a section based on its properties.
std::string assignRoomSection(const RoomListItem& room);

// Sort rooms within sections: favourites → unread → activity → name.
void sortRoomList(std::vector<RoomListItem>& rooms);

// Compute the composite priority score for a room.
int computeRoomPriority(const RoomListItem& room);

// Filter rooms by search query.
std::vector<RoomListItem> searchRoomList(const std::vector<RoomListItem>& rooms, const std::string& query);

// Format room list stats as JSON.
std::string roomListLayoutToJson(const RoomListLayout& layout);

// Format a single room list item for preview.
std::string formatRoomListItem(const RoomListItem& room);

// Get the notification badge text: empty, "N", or "N+".
std::string getBadgeText(const RoomListItem& room, int maxDisplay = 99);

// ==== Notification State (Element Web algorithm) ====

enum class NotificationLevel { NONE, GREY, RED };

struct NotificationState {
    NotificationLevel level = NotificationLevel::NONE;
    int count = 0;             // number to display
    std::string badgeText;     // "3", "99+", or ""
    bool showBadge = false;
};

// Compute notification state for a room (Element Web Badge logic):
//   - If has highlights → RED badge with highlight count
//   - If has notifications but muted → GREY badge  
//   - If has notifications and not muted → RED badge with notification count
//   - Otherwise → no badge
NotificationState computeNotificationState(const RoomListItem& room);

// Format the notification state as JSON for Kotlin UI.
std::string notificationStateToJson(const NotificationState& state);

// ==== New Room List Utilities ====

// Original Kotlin (RoomListManager.kt sortRoomList with mode):
//   Sort a list of rooms using the specified sort mode.
//   ACTIVITY: sort by lastActivityTs descending
//   ALPHABETICAL: sort by name ascending (case-insensitive)
//   PRIORITY: sort by computeRoomPriority descending
void sortRoomList(std::vector<RoomListItem>& rooms, RoomSortMode mode);

// Original Kotlin (RoomListManager.kt groupRoomList):
//   Group rooms into sections with headers for display.
//   Returns a vector of RoomListSection, each with its title and room list.
//   Section ordering: Favourites → Directs → Rooms → Spaces → Invites → Low Priority
std::vector<RoomListSection> groupRoomList(const std::vector<RoomListItem>& rooms);

// Original Kotlin (RoomListManager.kt buildRoomListSnapshot):
//   Build a complete display-ready room list with section headers,
//   grouped rooms, and metadata for the UI recycler view / list adapter.
std::vector<RoomGroupHeader> buildRoomListSnapshot(
    const std::vector<RoomListItem>& rooms,
    const std::string& filterQuery = ""
);

// Original Kotlin (RoomListManager.kt getRoomListSection):
//   Determine which category/section a room belongs to.
//   Overloaded version: returns a RoomGroupHeader for the room's section.
RoomGroupHeader getRoomListSection(const RoomListItem& room);

// Original Kotlin (RoomListManager.kt isRoomInMultipleSpaces):
//   Check if a room belongs to more than one space.
//   Used to determine if a room should show space badges in the list.
bool isRoomInMultipleSpaces(
    const std::string& roomId,
    const std::vector<std::pair<std::string, std::vector<std::string>>>& spaceChildren
);

// Original Kotlin (RoomSummaryFormatter.kt formatRoomLastActivity):
//   Format a relative time string for the room's last activity timestamp.
//   Returns strings like: "2m ago", "1h ago", "Yesterday", "Mon", "Jan 5"
//   Uses the same logic as Element Android's DateFormatter
std::string formatRoomLastActivity(int64_t lastActivityTs, int64_t nowMs = 0);

// Original Kotlin: Serialize RoomListSection to JSON for JNI bridge
std::string roomListSectionToJson(const RoomListSection& section);

// Original Kotlin: Serialize RoomGroupHeader to JSON for JNI bridge
std::string roomGroupHeaderToJson(const RoomGroupHeader& header);

// Original Kotlin: Serialize full room list snapshot (headers + sections) to JSON
std::string roomListSnapshotToJson(const std::vector<RoomListSection>& sections);

} // namespace progressive
