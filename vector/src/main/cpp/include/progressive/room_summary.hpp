#ifndef PROGRESSIVE_ROOM_SUMMARY_HPP
#define PROGRESSIVE_ROOM_SUMMARY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Room Summary ----

struct RoomSummaryInfo {
    std::string roomId;
    std::string displayName;       // room name or DM partner
    std::string avatarUrl;
    std::string topic;
    std::string canonicalAlias;

    // Membership
    bool isDirect = false;
    bool isJoined = false;
    bool isInvited = false;
    bool isLeft = false;
    bool isPublic = false;
    bool isSpace = false;
    bool isFavourite = false;
    bool isLowPriority = false;

    // Activity
    int notificationCount = 0;
    int highlightCount = 0;
    bool hasUnread = false;
    bool hasMention = false;
    std::string lastMessageBody;
    std::string lastMessageSender;
    int64_t lastMessageTs = 0;
    int64_t lastActivityTs = 0;

    // Encryption
    bool isEncrypted = false;
    std::string encryptionAlgorithm;

    // Membership stats
    int joinedMembers = 0;
    int invitedMembers = 0;

    // Draft
    bool hasDraft = false;
    std::string draftPreview;

    // Derived
    bool isPinned = false;
    int sortPriority = 0;  // higher = more important
};

struct RoomListStats {
    int totalRooms = 0;
    int totalUnread = 0;
    int totalHighlights = 0;
    int totalDirectChats = 0;
    int totalGroupRooms = 0;
    int totalSpaces = 0;
    int totalInvites = 0;
    int totalFavourites = 0;
};

// Compute room list statistics from summaries.
RoomListStats computeRoomListStats(const std::vector<RoomSummaryInfo>& rooms);

// Sort rooms by priority: favourites first, then unread, then activity, then name.
void sortRoomsByPriority(std::vector<RoomSummaryInfo>& rooms);

// Sort rooms by activity (most recent first).
void sortRoomsByActivity(std::vector<RoomSummaryInfo>& rooms);

// Sort rooms alphabetically.
void sortRoomsByName(std::vector<RoomSummaryInfo>& rooms);

// Compute a sort priority score for a room.
int computeRoomPriority(const RoomSummaryInfo& room);

// Format a room summary for the room list preview.
std::string formatRoomPreview(const RoomSummaryInfo& room, bool showSender);

// Format the last message preview with sender prefix.
std::string formatLastMessagePreview(const std::string& sender, const std::string& body, bool isEncrypted);

// Check if a room needs attention (pings, invites).
bool needsAttention(const RoomSummaryInfo& room);

// Get the notification badge text (empty, count, or "!" for highlights).
std::string getNotificationBadge(const RoomSummaryInfo& room);

// Format room list stats as JSON.
std::string roomListStatsToJson(const RoomListStats& stats);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_SUMMARY_HPP
