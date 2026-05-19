#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

namespace progressive {

// ================================================================
// Thread Manager — full Matrix thread lifecycle management
//
// Ported from Element Android:
//   ThreadListViewModel.kt, ThreadTimelineViewModel.kt
//   ThreadSummary.kt, ThreadsManager.kt
//   ThreadSummaryHelper.kt, ThreadEventsHelper.kt
//   ThreadsAwarenessHandler.kt
//
// Covers:
//   1. Thread root detection (m.thread relation)
//   2. Thread list enumeration (from room events)
//   3. Thread unread counter (read receipt based)
//   4. Thread notification state (highlight/hasMention)
//   5. Thread ordering (by latest activity, unread first)
//   6. Thread participant tracking
//   7. Thread timeline pagination state
//   8. Thread reply count
//   9. Thread summary computation (REPLACE/ADD modes)
//  10. Thread-aware event injection (fallback replies)
//  11. Thread pagination request/response parsing
//  12. Thread notification badge computation
// ================================================================

// ---- Thread State ----

enum class ThreadState {
    ACTIVE = 0,          // Normal thread with replies
    STALE = 1,           // No recent activity
    UNREAD = 2,          // Has unread messages
    HIGHLIGHTED = 3,     // Has @mention or highlight
};

// ---- Thread Info (full) ----

struct ThreadInfoFull {
    std::string threadId;            // Root event ID
    std::string roomId;
    std::string rootSenderId;
    std::string rootSenderName;
    std::string rootBody;            // First message text (truncated)
    std::string rootEventType;       // "m.room.message", etc.
    int64_t rootTimestampMs = 0;     // When root was sent
    int replyCount = 0;              // Total replies (excluding root)
    int64_t lastReplyTimestampMs = 0;
    std::string lastReplySenderId;
    std::string lastReplySenderName;
    std::string lastReplyBody;       // Preview of latest reply
    bool isUnread = false;
    bool hasHighlight = false;       // @mention or highlight
    int unreadCount = 0;             // Number of unread messages
    ThreadState state = ThreadState::ACTIVE;
    std::unordered_set<std::string> participantIds;
    bool rootIsEncrypted = false;
    std::string rootRelationType;    // "m.thread" or "m.annotation"
    std::string latestThreadEdition; // For edited thread roots
    bool valid = false;
};

// ---- Thread List ----

struct ThreadList {
    std::vector<ThreadInfoFull> threads;
    std::string nextBatch;           // Pagination token
    bool hasMore = false;            // More threads to load
    int totalCount = 0;
    int unreadCount = 0;
    int highlightedCount = 0;
};

// ---- Thread Event ----
// An event that belongs to a thread (either root or reply)

struct ThreadEvent {
    std::string eventId;
    std::string eventType;           // "m.room.message", etc.
    std::string senderId;
    std::string senderName;
    std::string body;
    std::string contentJson;         // Full event content
    int64_t timestampMs = 0;
    std::string threadRootId;        // Which thread this belongs to
    bool isThreadRoot = false;       // This IS the root
    bool isReply = false;            // This is a reply to a root
    bool isEncrypted = false;
    bool isEdit = false;
    bool isRedacted = false;
    int displayIndex = 0;           // Position in thread timeline
};

// ---- Thread Unread State ----

struct ThreadUnreadState {
    std::string threadId;
    int unreadCount = 0;
    bool highlighted = false;
    int64_t readReceiptPosition = 0; // Last read event index
};

// ---- Thread Notification ----

struct ThreadNotification {
    std::string threadId;
    std::string roomId;
    std::string title;               // "Alice: Hello everyone..."
    int unreadCount = 0;
    bool highlighted = false;
    int64_t timestampMs = 0;
};

// ================================================================
// NEW: Expanded Thread Models
// Ported from:
//   ThreadSummary.kt
//   ThreadSummaryUpdateType.kt
//   ThreadTimelineEvent.kt
//   ThreadNotificationState.kt
//   ThreadNotificationBadgeState.kt
//   ThreadEditions.kt
//   ThreadFilter.kt
// ================================================================

// ---- ThreadSummaryUpdateType ----
// Original Kotlin: ThreadSummaryUpdateType.kt

enum class ThreadSummaryUpdateType {
    REPLACE = 0,          // Full replacement from server (/messages with thread filter)
    ADD = 1               // Incremental add from sync
};

// ---- ThreadNotificationState ----
// Original Kotlin: ThreadNotificationState.kt

enum class ThreadNotificationState {
    NO_NEW_MESSAGE = 0,
    NEW_MESSAGE = 1,
    NEW_HIGHLIGHTED_MESSAGE = 2
};

// ---- ThreadNotificationBadgeState ----
// Original Kotlin: ThreadNotificationBadgeState.kt

struct ThreadNotificationBadgeState {
    int numberOfLocalUnreadThreads = 0;
    bool isUserMentioned = false;
};

// ---- SenderInfo ----
// Original Kotlin: SenderInfo (api/session/room/sender/)

struct SenderInfo {
    std::string senderId;
    std::string senderName;
    std::string avatarUrl;
    bool isUniqueDisplayName = true;
};

// ---- ThreadEditions ----
// Original Kotlin: ThreadEditions.kt

struct ThreadEditions {
    std::string rootThreadEdition;     // Edited text for root message
    std::string latestThreadEdition;   // Edited text for latest reply
};

// ---- Thread Filter ----
// Original Kotlin: ThreadFilter.kt

enum class ThreadFilter {
    ALL = 0,
    PARTICIPATED = 1
};

// ---- ThreadSummary ----
// Original Kotlin: ThreadSummary.kt
// Higher-level model matching the Kotlin API surface exactly.
// threadId is the rootEventId.

struct ThreadSummary {
    std::string roomId;
    std::string rootEventId;            // The thread identifier (= root event ID)
    std::string rootEventJson;          // Raw root event content JSON
    std::string latestEventId;          // Latest reply event ID
    std::string latestEventJson;        // Raw latest event content JSON
    SenderInfo rootThreadSenderInfo;    // Display name, avatar for root sender
    SenderInfo latestThreadSenderInfo;  // Display name, avatar for latest sender
    bool isUserParticipating = false;   // Current user has sent a message in this thread
    int numberOfThreads = 0;            // Total replies (excluding root)
    int highlightCount = 0;             // Number of messages with @mention
    int notificationCount = 0;          // Number of unread notifications
    ThreadEditions threadEditions;      // Edited versions of messages
    ThreadSummaryUpdateType updateType = ThreadSummaryUpdateType::ADD;
    ThreadNotificationState notificationState = ThreadNotificationState::NO_NEW_MESSAGE;
    int64_t rootTimestampMs = 0;        // origin_server_ts of root event
    int64_t latestTimestampMs = 0;      // origin_server_ts of latest event
    std::string latestBody;             // Decrypted text body of latest reply
    std::string rootBody;               // Decrypted text body of root message
    bool valid = false;                 // True if this summary is fully populated
};

// ---- ThreadTimelineEvent ----
// Original Kotlin: ThreadTimelineEvent.kt
// An event within a thread timeline, with thread-specific context.

struct ThreadTimelineEvent {
    std::string eventId;
    std::string eventType;              // "m.room.message", etc.
    std::string senderId;
    std::string senderName;
    std::string body;                   // Decrypted plaintext body
    std::string contentJson;            // Full event content JSON
    int64_t timestampMs = 0;            // origin_server_ts
    std::string rootThreadEventId;      // Which thread this belongs to
    bool isThreadRoot = false;          // This event IS the thread root
    bool isParticipating = false;       // Current user is a participant in this thread
    bool isEncrypted = false;
    bool valid = false;
};

// ---- ThreadPaginationResponse ----
// Original Kotlin: ThreadSummariesResponse.kt + RelationsResponse
// Parsed result from /relations or /messages API.

struct ThreadPaginationResponse {
    std::vector<std::string> events;    // Raw event JSON strings (the "chunk")
    std::string nextBatch;              // Pagination token for next page
    std::string prevBatch;              // Pagination token for previous page
    bool hasMore = false;               // True if there's another page
    bool valid = false;
};

// ---- FetchThreadsResult ----
// Original Kotlin: FetchThreadsResult.kt

struct FetchThreadsResult {
    bool reachedEnd = false;
    std::string nextBatch;
};

// ---- Thread Manager ----

class ThreadManager {
public:
    ThreadManager();

    // ====== Thread Root Detection ======

    // Check if an event is a thread root (has m.thread relation pointing to itself).
    bool isThreadRoot(const std::string& eventContentJson, const std::string& eventId);

    // Check if an event is a thread reply (has m.in_reply_to with thread root).
    // Returns the thread root event ID if it is a reply, empty string otherwise.
    std::string getThreadRootId(const std::string& eventContentJson);

    // Extract thread root from m.relates_to in event content.
    std::string extractThreadRoot(const std::string& eventContentJson);

    // ====== Thread Creation & Management ======

    // Add or update a thread (called when a new thread root event is seen).
    void upsertThread(const ThreadInfoFull& thread);

    // Add a reply to a thread.
    void addReply(const std::string& threadId, const std::string& senderId,
                  const std::string& senderName, const std::string& body,
                  int64_t timestampMs);

    // Remove a thread.
    void removeThread(const std::string& threadId);

    // Clear all threads for a room.
    void clearRoom(const std::string& roomId);

    // ====== Thread Queries ======

    // Get a thread by ID.
    bool getThread(const std::string& threadId, ThreadInfoFull& out) const;

    // Get all threads for a room.
    std::vector<ThreadInfoFull> getRoomThreads(const std::string& roomId) const;

    // Get thread list (sorted: unread first, then by latest activity).
    ThreadList getThreadList(int limit = 20, int offset = 0) const;

    // Get thread count for a room.
    int getRoomThreadCount(const std::string& roomId) const;

    // ====== Unread State ======

    // Set unread state for a thread.
    void setThreadUnread(const std::string& threadId, int unreadCount, bool highlighted);

    // Mark a thread as read.
    void markThreadRead(const std::string& threadId, int64_t readReceiptPosition);

    // Get unread state for a thread.
    ThreadUnreadState getUnreadState(const std::string& threadId) const;

    // Get total unread count across all threads.
    int getTotalUnreadCount() const;

    // ====== Notification ======

    // Get all threads that have notifications (unread + highlighted).
    std::vector<ThreadNotification> getNotifications() const;

    // Format thread notification count as a string ("12", "99+").
    static std::string formatThreadNotificationCount(int count);

    // ====== Thread Ordering ======

    // Sort threads: unread first, then highlighted, then by lastReplyTimestamp (descending).
    void sortThreads(std::vector<ThreadInfoFull>& threads) const;

    // ====== Serialization ======

    // Format thread info as JSON.
    std::string threadToJson(const ThreadInfoFull& thread) const;

    // Format thread list as JSON.
    std::string threadListToJson(const ThreadList& list) const;

    // Format unread state as JSON.
    std::string unreadStateToJson(const ThreadUnreadState& state) const;

    // Format notification as JSON.
    std::string notificationToJson(const ThreadNotification& notif) const;

    // ====== NEW: Thread Summary Support ======
    // Original Kotlin: ThreadSummaryHelper.kt

    // Compute and store a ThreadSummary from server-provided event JSON.
    // updateType=REPLACE: full replacement of thread metadata.
    // updateType=ADD: incremental update from a new event in the thread.
    ThreadSummary computeAndStoreSummary(const std::string& roomId,
                                         const std::string& rootEventJson,
                                         const std::string& latestEventJson,
                                         bool isUserParticipating,
                                         const std::string& userId);

    // Update a stored summary incrementally (ADD mode, from sync events).
    void updateStoredSummary(const std::string& threadId,
                             const std::string& eventJson,
                             ThreadSummaryUpdateType type,
                             const std::string& userId);

    // Get all stored ThreadSummary objects.
    std::vector<ThreadSummary> getAllSummaries() const;

    // Get ThreadSummary objects for a specific room.
    std::vector<ThreadSummary> getRoomSummaries(const std::string& roomId) const;

    // Get summaries that have unread messages or highlights.
    std::vector<ThreadSummary> getNotificationSummaries() const;

    // Get a single summary by thread ID.
    bool getSummary(const std::string& threadId, ThreadSummary& out) const;

    // ====== NEW: Thread Notification State ======
    // Original Kotlin: ThreadEventsHelper.updateThreadNotifications()

    // Compute and update notification state for a given thread.
    void updateThreadNotifications(const std::string& roomId,
                                   const std::string& threadId,
                                   const std::string& currentUserId);

    // Get the notification badge state across all threads.
    ThreadNotificationBadgeState getNotificationBadgeState(const std::string& userId) const;

    // ====== NEW: Static Event Inspection Utilities ======
    // Original Kotlin: ThreadsAwarenessHandler, ThreadEventsHelper

    // Check if event content JSON has a thread relation (m.thread).
    // Original Kotlin: ThreadsAwarenessHandler.isThreadEvent()
    static bool hasThreadRelation(const std::string& eventContentJson);

    // Check if event content JSON is a thread root event
    // (has m.thread relation pointing to its own eventId).
    static bool checkIsThreadRoot(const std::string& eventContentJson,
                                  const std::string& eventId);

    // Extract the root thread event ID from event content.
    // Original Kotlin: getRootThreadEventId()
    static std::string extractThreadRootId(const std::string& eventContentJson);

    // Format a human-readable preview string for the thread list.
    // Returns "SenderName: first line of message..."
    static std::string formatPreview(const std::string& senderName,
                                     const std::string& body,
                                     int maxLen = 60);

    // ====== Stats ======

    int totalThreads() const { return static_cast<int>(threads_.size()); }
    int totalRoomsWithThreads() const;

private:
    // Existing storage
    std::unordered_map<std::string, ThreadInfoFull> threads_;       // threadId → ThreadInfoFull
    std::unordered_map<std::string, ThreadUnreadState> unread_;     // threadId → unread state
    std::unordered_map<std::string, std::vector<ThreadEvent>> replies_; // threadId → replies
    std::unordered_map<std::string, int64_t> readReceipts_;         // threadId → last read index

    // NEW: ThreadSummary storage
    std::unordered_map<std::string, ThreadSummary> summaries_;      // threadId → ThreadSummary

    // JSON extract helpers
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static std::string extractObj(const std::string& json, const std::string& key);
    static std::string extractArray(const std::string& json, const std::string& key);
};

// ================================================================
// Free Function Declarations — Thread models & computation
// Ported from ThreadSummaryHelper.kt, ThreadEventsHelper.kt,
// ThreadsAwarenessHandler.kt, FetchThreadTimelineTask.kt
// ================================================================

// ---- Thread Summary Computation ----
// Original Kotlin: ThreadSummaryHelper.kt compute operations

// Build a ThreadSummary from raw event JSON.
ThreadSummary computeThreadSummary(const std::string& roomId,
                                   const std::string& rootEventJson,
                                   const std::string& latestEventJson,
                                   bool isUserParticipating,
                                   const std::string& userId);

// Update an existing ThreadSummary with a new event.
// Original Kotlin: ThreadSummaryEntity.updateThreadSummary()
void updateThreadSummary(ThreadSummary& summary,
                         const std::string& eventJson,
                         ThreadSummaryUpdateType type,
                         const std::string& userId);

// Format a human-readable preview for the thread list.
// Original Kotlin: formatting logic from ThreadListViewModel
std::string formatThreadPreview(const ThreadSummary& summary);

// Check if an event has a thread relation (m.thread).
// Original Kotlin: ThreadsAwarenessHandler.isThreadEvent()
bool isThreadEvent(const std::string& eventContentJson);

// ---- Thread Event Helpers ----
// Original Kotlin: ThreadEventsHelper.kt

// Find the root thread event ID by traversing m.relates_to relations.
// Original Kotlin: EventEntity.findRootThreadEvent()
std::string findRootThreadEventId(const std::string& eventContentJson);

// Get the thread relation content from an event as JSON string.
// Returns the m.relates_to object with rel_type="m.thread".
// Original Kotlin: getRootThreadRelationContent()
std::string getThreadEventRelation(const std::string& eventContentJson);

// Make an event thread-aware: if it's a thread reply, inject
// the parent message as a reply quote in the content.
// Returns modified event content JSON, or empty string on failure.
// Original Kotlin: ThreadsAwarenessHandler.makeEventThreadAware()
std::string makeEventThreadAware(const std::string& roomId,
                                 const std::string& eventContentJson,
                                 const std::string& parentEventJson = "");

// Check if an event belongs to a specific thread.
// Original Kotlin: check if rootThreadEventId matches
bool isEventInThread(const std::string& eventContentJson,
                     const std::string& threadRootId);

// ---- Thread Pagination ----
// Original Kotlin: FetchThreadTimelineTask.kt, FetchThreadSummariesTask.kt

// Build the URL path for fetching thread events via /relations API.
// GET /_matrix/client/v3/rooms/{roomId}/relations/{eventId}/m.thread
std::string buildThreadRelationsUrl(const std::string& roomId,
                                    const std::string& rootEventId,
                                    const std::string& from = "",
                                    int limit = 20);

// Build the URL path for fetching thread summaries via /messages API.
// GET /_matrix/client/v3/rooms/{roomId}/messages?filter={"types":[...]}
std::string buildThreadSummariesUrl(const std::string& roomId,
                                    const std::string& from = "",
                                    int limit = 5,
                                    const std::string& filter = "all");

// Build the JSON request body for fetching thread timeline events.
std::string buildThreadTimelineRequest(const std::string& roomId,
                                       const std::string& rootEventId,
                                       const std::string& from = "",
                                       int limit = 20);

// Parse a pagination response (from /relations or /messages API).
// Handles both ThreadSummariesResponse and RelationsResponse formats.
ThreadPaginationResponse parseThreadTimelineResponse(const std::string& responseJson);

// ---- Thread Notification ----
// Original Kotlin: ThreadEventsHelper.updateThreadNotifications()

// Compute the notification state for a thread.
ThreadNotificationState computeThreadNotifications(int unreadCount,
                                                    bool isUserMentioned,
                                                    bool isUserParticipating);

// Compute a badge state from a list of thread summaries.
ThreadNotificationBadgeState computeThreadBadgeState(
    const std::vector<ThreadSummary>& summaries,
    bool isUserMentioned);

// ---- Thread Root Detection ----
// Original Kotlin: isRootThread check

// Extract the thread root event ID from an event's m.relates_to.
std::string extract_thread_root_id(const std::string& eventContentJson);

// Build a JSON string for an m.thread relation.
// {"rel_type":"m.thread","event_id":"<rootEventId>"}
std::string buildThreadRootRelation(const std::string& rootEventId,
                                    bool isFallingBack = false);

// Check if an event is a thread root (has m.thread relation
// pointing to itself OR has replies referencing it).
// Original Kotlin: EventEntity.isRootThread()
bool isThreadRootEvent(const std::string& eventContentJson,
                       const std::string& eventId);

// ---- Type Conversion ----

// Convert ThreadSummary to ThreadInfoFull (backward compatibility).
ThreadInfoFull threadSummaryToInfoFull(const ThreadSummary& summary);

// Convert ThreadInfoFull to ThreadSummary.
ThreadSummary infoFullToThreadSummary(const ThreadInfoFull& info);

// ---- Serialization ----

// Format ThreadSummary as JSON.
std::string threadSummaryToJson(const ThreadSummary& summary);

// Format ThreadTimelineEvent as JSON.
std::string threadTimelineEventToJson(const ThreadTimelineEvent& event);

// Format a vector of ThreadSummary as a JSON array.
std::string threadSummaryListToJson(const std::vector<ThreadSummary>& list);

} // namespace progressive
