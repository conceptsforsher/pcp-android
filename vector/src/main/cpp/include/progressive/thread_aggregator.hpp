#ifndef PROGRESSIVE_THREAD_AGGREGATOR_HPP
#define PROGRESSIVE_THREAD_AGGREGATOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Thread Header Metadata ----
// Computes the header display for the thread view:
//   [←] Discussion: "<thread title>"  [mini avatar]  [⋯]
//              12 messages
//
// Original Kotlin: ThreadHeaderView.kt, ThreadSummary.kt

struct ThreadMeta {
    std::string rootEventId;       // root event of the thread
    std::string rootBody;          // first message body (truncated for title)
    std::string rootSenderId;      // who started the thread
    std::string rootSenderName;    // display name of thread starter
    std::string rootSenderAvatar;  // mxc:// avatar URL of starter
    int replyCount = 0;            // total replies (excluding root)
    int64_t lastReplyTs = 0;       // epoch ms of latest reply
    std::string lastReplySenderName;
    std::string lastReplyBody;     // preview of last reply
    bool isUnread = false;
    bool valid = false;            // has rootEventId + rootBody

    // Format "N messages" / "1 message"
    std::string messageCountText() const;
    // Format truncated title: "Alice: Hello everyone..." (max 60 chars)
    std::string title() const;
};

// Compute thread metadata from events.
// Takes: root event content, list of reply events (sender, body, ts)
ThreadMeta computeThreadMeta(
    const std::string& rootEventContent,
    const std::vector<std::string>& replySenders,
    const std::vector<std::string>& replyBodies,
    const std::vector<int64_t>& replyTimestamps
);

// Format thread metadata as JSON for Kotlin UI.
std::string threadMetaToJson(const ThreadMeta& meta);

struct ThreadInfo {
    std::string threadId;
    std::string roomId;
    std::string roomName;
    std::string accountId;
    std::string accountIndex;
    std::string lastMessage;
    std::string lastSender;
    int64_t lastTimestamp = 0;
    int replyCount = 0;
    bool unread = false;
};

class ThreadAggregator {
public:
    // Add or update a thread from a room.
    void addThread(const ThreadInfo& thread);

    // Remove all threads for a room.
    void removeRoom(const std::string& roomId);

    // Remove all threads for an account.
    void removeAccount(const std::string& accountId);

    // Get all threads, sorted by last activity (most recent first).
    std::vector<ThreadInfo> getAllThreads() const;

    // Get thread count.
    size_t count() const { return threads_.size(); }

    // Clear all.
    void clear();

    // Export as JSON.
    std::string exportJson() const;

private:
    // key: threadId → ThreadInfo
    std::unordered_map<std::string, ThreadInfo> threads_;
};

} // namespace progressive

#endif // PROGRESSIVE_THREAD_AGGREGATOR_HPP
