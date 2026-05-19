#ifndef PROGRESSIVE_MESSAGE_QUEUE_HPP
#define PROGRESSIVE_MESSAGE_QUEUE_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Message Deduplication ----

struct DedupResult {
    bool isDuplicate = false;
    std::string originalEventId;  // which event this duplicates
    int duplicateCount = 0;       // how many times seen
};

// Check if an event body is a duplicate of a recently sent message.
// Uses fuzzy matching: identical after stripping whitespace and normalizing.
DedupResult checkDuplicate(
    const std::string& newBody,
    const std::vector<std::string>& recentBodies,
    double threshold = 0.95  // similarity threshold (0.0-1.0)
);

// Compute text similarity (0.0-1.0) using character trigram overlap.
double textSimilarity(const std::string& a, const std::string& b);

// Normalize text for comparison: lowercase, trim, collapse whitespace.
std::string normalizeForComparison(const std::string& text);

// ---- Message Batching ----

struct BatchedMessage {
    std::string body;
    int64_t timestampMs = 0;
    bool isContinuation = false; // same sender as previous
};

// Batch consecutive messages from the same sender into logical blocks.
// Messages within a short time window from the same sender are marked as continuations.
std::vector<BatchedMessage> batchMessages(
    const std::vector<std::string>& bodies,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestamps,
    int64_t mergeWindowMs = 300000  // 5 minutes
);

// ---- Message Pinning Logic ----

struct PinnedMessage {
    std::string eventId;
    std::string body;
    std::string senderName;
    int64_t pinnedAtMs = 0;
    bool isExpired = false;    // pin has a TTL
    int64_t expiresAtMs = 0;   // 0 = never expires
};

class PinManager {
public:
    void pin(const PinnedMessage& msg);
    void unpin(const std::string& eventId);
    std::vector<PinnedMessage> getActivePins() const;
    void checkExpired();
    std::string exportJson() const;
    void clear();
    int count() const { return static_cast<int>(pins_.size()); }

private:
    std::vector<PinnedMessage> pins_;
};

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_QUEUE_HPP
