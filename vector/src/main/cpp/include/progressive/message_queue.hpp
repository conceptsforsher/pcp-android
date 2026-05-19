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

// ---- Send Queue Models ----
// Ported from: org.matrix.android.sdk.api.session.room.send.SendState
//              org.matrix.android.sdk.internal.session.room.send.LocalEchoIdentifiers
//              org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessor

// Original Kotlin: SendState enum in SendState.kt
enum class SendState {
    UNSENT,
    SENDING,
    SENT,
    FAILED,
    UNDELIVERABLE,
    SENT_FROM_ANOTHER_SESSION
};

// Convert SendState to string (e.g. for JSON serialization)
const char* sendStateToString(SendState state);

// Parse SendState from string
SendState sendStateFromString(const std::string& str);

// Original Kotlin: LocalEchoIdentifiers + SendEventQueuedTask
// Represents an item in the send queue
struct QueueEntry {
    std::string localId;         // transaction/local event ID (eventId in Kotlin)
    std::string event;           // JSON serialized event body
    std::string roomId;          // target room
    SendState sendState = SendState::UNSENT;
    int retryCount = 0;
    int64_t lastAttemptMs = 0;   // timestamp of last send attempt
    int priority = 0;            // lower = higher priority (0 = normal, <0 = high)
};

// Statistics about the current send queue state
struct SendQueueStats {
    int pending = 0;
    int sending = 0;
    int failed = 0;
    int sent = 0;
    int total() const { return pending + sending + failed + sent; }
};

// Original Kotlin: EventSenderProcessor / EventSenderProcessorCoroutine
// Manages a priority-ordered send queue for events.
// Events are dequeued by priority (lower first), then by oldest first.
class MessageQueue {
public:
    // Add an event to the queue. Lower priority values are sent first.
    void enqueue(const QueueEntry& entry);

    // Remove and return the highest-priority entry. Returns empty QueueEntry if empty.
    QueueEntry dequeue();

    // Look at the next entry without removing.
    QueueEntry peek() const;

    // Remove a specific entry by localId. Returns true if found and removed.
    bool remove(const std::string& localId);

    // Get all entries sorted by priority then by enqueue time.
    std::vector<QueueEntry> getAll() const;

    // Number of entries in the queue.
    int count() const;

    // Get entries in FAILED or UNDELIVERABLE state.
    std::vector<QueueEntry> getFailedEvents() const;

    // Get entries in UNSENT or SENDING state.
    std::vector<QueueEntry> getPendingEvents() const;

    // Update state of a specific entry by localId. Returns true if found.
    bool markAsSending(const std::string& localId);
    bool markAsSent(const std::string& localId);
    bool markAsFailed(const std::string& localId);

    // Get queue statistics.
    SendQueueStats getStats() const;

    // Remove all entries from the queue.
    void clear();

private:
    std::vector<QueueEntry> entries_;
};

// ---- Send Event Builders ----
// Ported from: org.matrix.android.sdk.internal.crypto.tasks.SendEventTask
//              org.matrix.android.sdk.internal.session.room.send.RedactEventWorker
//              org.matrix.android.sdk.internal.crypto.tasks.RedactEventTask

// Original Kotlin: SendEventTask.Params → PUT /_matrix/client/v3/rooms/{roomId}/send/{eventType}/{txnId}
// Build the full JSON request body for sending an event.
std::string buildSendEventRequest(
    const std::string& roomId,
    const std::string& eventType,
    const std::string& txnId,
    const std::string& contentJson
);

// Build the event JSON body (type, content, room_id, txn_id).
std::string buildSendEventBody(
    const std::string& eventType,
    const std::string& contentJson,
    const std::string& roomId,
    const std::string& txnId
);

// Original Kotlin: RedactEventTask.Params → PUT /_matrix/client/v3/rooms/{roomId}/redact/{eventId}/{txnId}
// Build the full JSON request body for redacting (deleting) an event.
std::string buildRedactEventRequest(
    const std::string& roomId,
    const std::string& eventId,
    const std::string& txnId,
    const std::string& reason = ""
);

// ---- Send Deduplication ----
// Ported from: org.matrix.android.sdk.api.session.events.model.LocalEcho

// Original Kotlin: LocalEcho.createLocalEchoId() uses UUID.randomUUID()
// Generates unique transaction IDs for send deduplication.
class TransactionIdGenerator {
public:
    // Generate a new unique transaction ID (UUID v4 style).
    static std::string generate();

    // Generate a deterministic txnId for retries, appending retry count
    // to the local ID so the server can dedup retries.
    static std::string generateForRetry(const std::string& localId, int retryCount);

private:
    static std::string randomHex(int bytes);
};

// Original Kotlin: Throwable.shouldBeRetried() from Extensions.kt
// Checks whether a send error should trigger a retry:
//   - Network timeout / connection error (isNetworkError=true) → retry
//   - 429 Rate Limited → retry
//   - 5xx Server Error → retry
//   - 4xx Client Error → do NOT retry
bool isRetryableSendError(int httpErrorCode, bool isNetworkError);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_QUEUE_HPP
