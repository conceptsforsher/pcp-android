#ifndef PROGRESSIVE_ROOM_SUMMARY_HPP
#define PROGRESSIVE_ROOM_SUMMARY_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint>

namespace progressive {

// ---- Room Summary ----

struct RoomSummaryInfo {
    std::string roomId;
    std::string displayName;       // room name or DM partner
    std::string avatarUrl;
    std::string topic;
    std::string canonicalAlias;
    std::string membership;        // "join", "invite", "leave", "ban", "knock"

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
    int threadNotificationCount = 0;   // threads with unread
    int threadHighlightCount = 0;      // threads with highlights
    bool hasUnread = false;
    bool hasMention = false;
    bool hasFailedSending = false;
    std::string lastMessageBody;
    std::string lastMessageSender;
    int64_t lastMessageTs = 0;
    int64_t lastActivityTs = 0;
    std::string readMarkerId;          // m.fully_read event ID

    // Encryption
    bool isEncrypted = false;
    std::string encryptionAlgorithm;
    int64_t encryptionEventTs = 0;

    // Membership stats
    int joinedMembers = 0;
    int invitedMembers = 0;

    // Versioning
    bool isVersioned = false;
    std::string versioningState;       // "none", "upgraded_room_joined", etc.

    // Draft
    bool hasDraft = false;
    std::string draftPreview;

    // Inviter (for invite rooms)
    std::string inviterId;

    // Derived
    bool isPinned = false;
    bool isHiddenFromUser = false;
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

// ==== Room Summary Data from Sync ====
//
// Ported from RoomSyncSummary.kt and RoomSyncUnreadNotifications.kt
// These are the fields extracted from /sync response to update room summaries.

struct RoomSyncData {
    // From RoomSyncSummary
    std::vector<std::string> heroes;        // hero user IDs for room avatar
    int joinedMemberCount = -1;             // -1 = unknown (null in Kotlin)
    int invitedMemberCount = -1;            // -1 = unknown

    // From RoomSyncUnreadNotifications
    int unreadNotificationCount = 0;        // notification_count from sync
    int unreadHighlightCount = 0;           // highlight_count from sync
};

struct ThreadUnreadData {
    std::string threadId;
    int notificationCount = 0;
    int highlightCount = 0;
};

// State event data needed for room summary update.
struct RoomStateEvents {
    std::string roomName;
    std::string roomTopic;
    std::string canonicalAlias;
    std::vector<std::string> aliases;
    std::string joinRule;                      // "public", "invite", etc.
    std::string roomType;                      // "m.space" or empty
    bool isEncrypted = false;
    std::string encryptionAlgorithm;
    bool hasEncryptionEvent = false;
    int64_t encryptionEventTs = 0;
};

// Previewable event filter configuration.
// Ported from RoomSummaryEventsHelper.kt + RoomSummaryConstants
struct RoomPreviewFilter {
    std::unordered_set<std::string> allowedTypes;   // previewable event types
    bool filterUseless = true;                       // skip redactions, etc.
    bool filterEdits = true;                         // skip m.replace events
    bool filterRedacted = false;                     // keep redacted events?
};

// ================================================================
// Room Summary Update Functions
//
// Ported from RoomSummaryUpdater.kt (update, refreshLatestPreviewContent,
// updateSendingInformation) and RoomSummaryEventsHelper.kt.
// ================================================================

// Update a room summary from sync data and state events.
// Original Kotlin: RoomSummaryUpdater.update() (lines 89-213)
RoomSummaryInfo updateRoomSummary(
    const std::string& roomId,
    const RoomSyncData& syncData,
    const RoomStateEvents& stateEvents,
    const std::vector<ThreadUnreadData>& threadUnreads,
    const std::string& inviterId,
    const std::string& readMarkerId,
    const std::string& latestEventId,
    int64_t latestEventTs
);

// Compute whether a room has unread messages.
// Original Kotlin: RoomSummaryUpdater lines 156-158
// notificationCount > 0 OR last event is not read
bool computeHasUnreadMessages(
    int notificationCount,
    const std::string& latestEventId,
    const std::string& readMarkerId,
    const std::string& userId,
    const std::string& roomId,
    bool canUseThreadReceipts
);

// Compute thread notification totals.
// Original Kotlin: RoomSummaryUpdater lines 116-122
// Counts threads with unread notifications and highlights.
void computeThreadNotificationCounts(
    const std::vector<ThreadUnreadData>& threadUnreads,
    int& outThreadNotificationCount,
    int& outThreadHighlightCount
);

// Check if a room should be hidden from user.
// Original Kotlin: RoomSummaryUpdater lines 129-130
// Hidden if versioned (upgraded) or is a virtual room.
bool shouldHideFromUser(
    bool isUpgradedRoomJoined,
    bool isVirtualRoom
);

// Build a default preview filter matching Element Android constants.
// Original Kotlin: RoomSummaryConstants.PREVIEWABLE_TYPES
RoomPreviewFilter buildDefaultPreviewFilter();

// Check if an event type should be previewable (shown in room list).
// Original Kotlin: RoomSummaryEventsHelper filtering logic
bool isPreviewableEventType(const std::string& eventType, const RoomPreviewFilter& filter);

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
