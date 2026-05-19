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
    std::string formatThreadNotificationCount(int count) const;

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

    // ====== Stats ======

    int totalThreads() const { return static_cast<int>(threads_.size()); }
    int totalRoomsWithThreads() const;

private:
    std::unordered_map<std::string, ThreadInfoFull> threads_;     // threadId → ThreadInfoFull
    std::unordered_map<std::string, ThreadUnreadState> unread_; // threadId → unread state
    std::unordered_map<std::string, std::vector<ThreadEvent>> replies_; // threadId → replies
    std::unordered_map<std::string, int64_t> readReceipts_;   // threadId → last read index

    // JSON extract helper
    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
};


} // namespace progressive
