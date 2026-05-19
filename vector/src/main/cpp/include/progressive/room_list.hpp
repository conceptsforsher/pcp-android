#ifndef PROGRESSIVE_ROOM_LIST_HPP
#define PROGRESSIVE_ROOM_LIST_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

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

} // namespace progressive

#endif // PROGRESSIVE_ROOM_LIST_HPP
