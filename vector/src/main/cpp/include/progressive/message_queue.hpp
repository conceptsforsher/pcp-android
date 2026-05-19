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

    // Update a specific entry (identified by localId) with new field values.
    // The updated entry replaces the existing one in the queue.
    // Returns true if the entry was found and updated.
    bool updateEntry(const std::string& localId, const QueueEntry& updated);

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

// ---- Send Orchestration ----
// Ported from: org.matrix.android.sdk.api.session.room.send.SendService
//              org.matrix.android.sdk.internal.session.room.send.DefaultSendService
//              org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessor

// Original Kotlin: SendService.sendEvent(eventType, content) → Cancelable
struct SendRequest {
    std::string roomId;
    std::string eventType;
    std::string contentJson;
    std::string transactionId;  // assigned by submitEvent
    int priority = 0;          // lower = higher priority (0 = normal, <0 = high)
};

// Original Kotlin: Event.result → SendState.SENT or UNDELIVERED
struct SendResult {
    std::string eventId;         // server-assigned event ID on success
    std::string transactionId;   // client-assigned transaction ID
    bool success = false;
    std::string errorMessage;
    int errorCode = 0;           // HTTP status code (0 = network error)
};

// Original Kotlin: EventSenderProcessorCoroutine + DefaultSendService
// Manages the full lifecycle of event sending: validation, encryption,
// submission, confirmation, and decoration.
class SendQueueManager {
public:
    // Submit an event for sending. Generates a transactionId and enqueues the event.
    // Returns the transactionId for tracking.
    std::string submitEvent(const SendRequest& request);

    // Cancel a specific event by transaction ID.
    // Returns true if the event was found and cancelled/removed.
    bool cancelEvent(const std::string& transactionId);

    // Cancel all pending events for a specific room.
    // Returns the number of events cancelled.
    int cancelAllInRoom(const std::string& roomId);

    // Get counts of various queue states.
    int getPendingCount() const;
    int getFailedCount() const;
    int getSendingCount() const;

    // Process the pending queue: dequeue UNSENT events in priority order,
    // mark them SENDING, and return results.
    // Each event runs through the send pipeline stages.
    std::vector<SendResult> processQueue();

    // Re-queue all FAILED/UNDELIVERABLE events for retry.
    // Sets their state back to UNSENT and resets attempt counters.
    // Returns the number of events re-queued.
    int requeueFailed();

    // Get all events currently in FAILED or UNDELIVERABLE state.
    std::vector<QueueEntry> getFailedEvents() const;

    // Get all events in UNSENT or SENDING state.
    std::vector<QueueEntry> getPendingEvents() const;

    // Get queue statistics (counts per state).
    SendQueueStats getStats() const;

    // Get the underlying queue entries (for persistence/memento).
    std::vector<QueueEntry> getAll() const;

    // Update the send state for a specific event by transactionId.
    bool updateState(const std::string& transactionId, SendState newState);

    // Check if an event with the given transactionId exists.
    bool hasEvent(const std::string& transactionId) const;

    // Clear all entries from the queue.
    void clear();

    // Get the number of total entries.
    int count() const;

private:
    MessageQueue queue_;
};

// Original Kotlin: LocalEchoRepository.updateSendState → tracks per-event transitions
// Tracks the full lifecycle: UNSENT → ENCRYPTING → SENDING → SENT (or FAILED/UNDELIVERED)
struct SendStateTracker {
    std::string eventId;
    std::string transactionId;
    std::string roomId;
    SendState currentState = SendState::UNSENT;
    int64_t stateChangedAtMs = 0;
    std::string errorMessage;
    int errorCode = 0;

    // Terminal states: no further transitions possible (except manual resend)
    bool isTerminal() const {
        return currentState == SendState::SENT ||
               currentState == SendState::SENT_FROM_ANOTHER_SESSION ||
               currentState == SendState::FAILED ||
               currentState == SendState::UNDELIVERABLE;
    }

    // In-flight states: event is still being processed
    bool isInFlight() const {
        return currentState == SendState::UNSENT ||
               currentState == SendState::SENDING;
    }

    // Has this event permanently failed?
    bool hasFailed() const {
        return currentState == SendState::FAILED ||
               currentState == SendState::UNDELIVERABLE;
    }

    // Transition to a new state and record the timestamp.
    void transitionTo(SendState newState, int64_t nowMs) {
        currentState = newState;
        stateChangedAtMs = nowMs;
    }
};

// ---- Send Pipeline ----
// Ported from: DefaultSendService sendEvent pipeline
//              + LocalEchoEventFactory.createLocalEcho (validation)
//              + CryptoService.encryptEvent (encryption)
//              + SendEventTask.execute (network send)
//              + localEchoRepository.updateSendState (confirmation)
//              + timeline decoration (decorate)

// Original Kotlin: Pipeline stages matching the send lifecycle
// Corresponds to: createLocalEcho → encrypt → PUT /send → updateState → decorate
enum class SendPipelineStage {
    VALIDATE,    // Check event content, room state, permissions
    ENCRYPT,     // Encrypt event content if room is encrypted
    SEND,        // Send to server via PUT /_matrix/client/v3/rooms/{roomId}/send/{eventType}/{txnId}
    CONFIRM,     // Update local echo state based on server response (event_id, send state)
    DECORATE     // Add timeline metadata (sender, timestamp, relations, age)
};

// Original Kotlin: Pipeline state carried through each stage
struct SendPipeline {
    SendRequest request;
    SendResult result;
    SendPipelineStage currentStage = SendPipelineStage::VALIDATE;
    int64_t startedAtMs = 0;
    int64_t completedAtMs = 0;
    bool encrypted = false;
    std::string encryptedContentJson;  // ciphertext content (set after ENCRYPT stage)

    // Advance to the next stage. Returns false if already at the last stage.
    bool advanceStage() {
        switch (currentStage) {
            case SendPipelineStage::VALIDATE: currentStage = SendPipelineStage::ENCRYPT; return true;
            case SendPipelineStage::ENCRYPT:  currentStage = SendPipelineStage::SEND; return true;
            case SendPipelineStage::SEND:     currentStage = SendPipelineStage::CONFIRM; return true;
            case SendPipelineStage::CONFIRM:  currentStage = SendPipelineStage::DECORATE; return true;
            case SendPipelineStage::DECORATE: return false;
        }
        return false;
    }
};

// Execute all pipeline stages for a send request sequentially.
// Returns the SendResult after all stages complete (or after first failure).
// roomIsEncrypted: if true, the ENCRYPT stage will be active.
SendResult runSendPipeline(const SendRequest& request, bool roomIsEncrypted = false);

// Validate event content before sending.
// Checks: roomId non-empty, eventType non-empty, contentJson is valid JSON.
// Returns true if the event is valid to send.
bool validateEvent(const SendRequest& request);

// Prepare an event for sending: generate transaction ID, add timestamp, format for server.
// Returns the prepared event body JSON string suitable for the PUT /send endpoint.
// Adds origin_server_ts and unsigned.transaction_id if not already present.
std::string prepareEventForSend(const SendRequest& request, const std::string& senderId);

// Convert SendState enum to its API-level string representation.
// Maps to the Kotlin SendState.name values: UNSENT, SENDING, SENT, etc.
const char* sendStateToApiString(SendState state);

// Check if a send state is one of the "has sent" states (SENT, SYNCED, SENT_FROM_ANOTHER_SESSION).
bool isSentState(SendState state);

// Check if a send state is one of the "has failed" states (UNDELIVERED, FAILED_UNKNOWN_DEVICES).
bool isFailedState(SendState state);

// Check if a send state is one of the "in progress" states (ENCRYPTING, SENDING).
bool isInProgressState(SendState state);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_QUEUE_HPP
