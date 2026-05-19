#ifndef PROGRESSIVE_MESSAGE_RETRY_HPP
#define PROGRESSIVE_MESSAGE_RETRY_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Message Retry Queue ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.QueueMediator.kt
//              org.matrix.android.sdk.internal.session.room.send.RetryDecider.kt
//              im.vector.app.features.home.room.detail.timeline.factory.MessageItemFactory.kt

enum class MessageSendState {
    Pending,         // queued, not yet sent
    Sending,         // currently being sent
    Sent,            // successfully sent
    Failed,          // failed (permanent error)
    Retrying,        // retrying after temporary failure
    Cancelled        // user cancelled
};

struct PendingMessage {
    std::string localId;          // local event ID (txnId)
    std::string roomId;
    std::string body;             // message content
    std::string msgType;          // "m.text", "m.image", "m.file", etc.
    int64_t queuedAtMs = 0;       // when was it queued
    int64_t lastAttemptMs = 0;    // last send attempt timestamp
    int retryCount = 0;           // number of retries so far
    MessageSendState state = MessageSendState::Pending;
    std::string error;            // last error message
    int errorCode = 0;            // HTTP status code (0 if unknown)
};

struct RetryDecision {
    bool shouldRetry = false;
    int64_t delayMs = 0;          // how long to wait before retrying
    std::string reason;           // why this decision was made
};

// Compute the retry delay using exponential backoff.
// Base delay 1s, doubles each retry, capped at 5 minutes.
// Original Kotlin (RetryDecider.kt):
//   fun computeRetryDelay(retryCount: Int, maxDelayMs: Long): Long
int64_t computeRetryDelay(int retryCount, int64_t maxDelayMs = 300000);

// Decide whether to retry sending a message based on error code.
// 429 Rate Limited → retry after Retry-After header
// 5xx Server Error → retry with backoff
// 4xx Client Error → don't retry (except 429)
// Network error → retry with backoff
RetryDecision decideRetry(const PendingMessage& msg, int errorCode, const std::string& retryAfterHeader = "");

// Update the message state after a send attempt.
PendingMessage afterAttempt(PendingMessage msg, bool success, int errorCode, const std::string& error, int64_t nowMs);

// Check if a message has been pending too long (stale).
bool isStaleMessage(const PendingMessage& msg, int64_t nowMs, int64_t maxAgeMs = 600000);

// Filter the retry queue: remove cancelled, mark stale as failed.
std::vector<PendingMessage> cleanQueue(const std::vector<PendingMessage>& queue, int64_t nowMs);

// Sort queue: oldest first, then by retry count (fewer retries first).
std::vector<PendingMessage> sortQueue(std::vector<PendingMessage> queue);

// Get the next message that should be sent (state == Pending or Retrying).
// Returns the one with the oldest queuedAtMs that has waited long enough.
PendingMessage getNextToSend(const std::vector<PendingMessage>& queue, int64_t nowMs);

// Format message state as JSON for the Kotlin UI layer.
std::string pendingMessageToJson(const PendingMessage& msg);

// Format the full queue as JSON array.
std::string queueToJson(const std::vector<PendingMessage>& queue);

// Get a human-readable status for a message.
std::string formatMessageStatus(MessageSendState state);

// Get a display text for the retry count badge.
std::string formatRetryBadge(int retryCount);

// ---- Pending Message Editing ----
// Allow editing messages that are still pending/sending.
// Original Element behavior: edit only after server confirms send.
// Progressive Chat: edit immediately, apply to pending queue.
// The edit is sent as an m.replace relation pointing to the pending event.

struct EditResult {
    bool success = false;
    std::string error;
    bool wasPending = false;       // message was still pending when edited
    bool wasSending = false;       // message was being sent when edited
};

// Edit a message that may still be pending/sending.
// Updates the body in the queue. If the message hasn't been sent yet,
// the edited body will be sent when the queue retries.
// If it's in progress, the edit event will be sent AFTER the original.
PendingMessage editPendingMessage(std::vector<PendingMessage>& queue, const std::string& localId, const std::string& newBody, int64_t nowMs);

// Check if a pending message CAN be edited (not failed/cancelled).
bool canEditPendingMessage(const PendingMessage& msg);

// ---- Retry Configuration ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessorCoroutine
//              org.matrix.android.sdk.api.failure.Extensions

// Original Kotlin: constants in EventSenderProcessorCoroutine
//   MAX_RETRY_COUNT = 3, RETRY_WAIT_TIME_MS = 10_000
struct RetryConfig {
    int maxRetries = 3;              // maximum retry attempts before failing permanently
    int64_t baseDelayMs = 1000;      // starting delay for exponential backoff
    int64_t maxDelayMs = 300000;     // delay cap (5 minutes)
    bool jitterEnabled = true;       // add randomness to spread retries
};

// Original Kotlin: WorkManager BackoffPolicy (LINEAR only, but we model all 3)
enum class RetryStrategy {
    LINEAR,                      // same delay every retry
    EXPONENTIAL,                 // delay doubles each retry
    EXPONENTIAL_WITH_JITTER      // exponential + random spread (±25%)
};

// Compute the retry delay using the configured strategy.
// EXPONENTIAL: baseDelayMs * 2^retryCount, capped at maxDelayMs.
// EXPONENTIAL_WITH_JITTER: exponential delay + random jitter (±25%).
// LINEAR: baseDelayMs (constant).
int64_t computeRetryDelay(int retryCount, const RetryConfig& config = RetryConfig{});

// Check if we should retry based on count vs config.maxRetries.
bool shouldRetry(int retryCount, const RetryConfig& config = RetryConfig{});

// ---- RetryDecider ----
// Original Kotlin: EventSenderProcessorCoroutine.exception handling in executeTask()
// Decides whether to retry based on error type, returning delay and reason.

class RetryDecider {
public:
    // Structured decision result
    struct Decision {
        bool shouldRetry = false;
        int64_t delayMs = 0;
        std::string reason;          // human-readable reason for UI / logging
    };

    // Decide retry for a given error. Considers:
    //   - 429 Rate Limited → retry with rateLimitRetryAfterMs + 100ms, or exponential
    //   - Network error → retry with exponential backoff
    //   - 5xx Server Error → retry with exponential backoff
    //   - 4xx Client Error → no retry
    // Respects config.maxRetries.
    Decision decide(int httpErrorCode, bool isNetworkError, int currentRetryCount,
                    int64_t rateLimitRetryAfterMs = 0,
                    const RetryConfig& config = RetryConfig{}) const;

    // Convenience: just get the delay (0 = don't retry)
    int64_t decideDelay(int httpErrorCode, bool isNetworkError, int currentRetryCount,
                        const RetryConfig& config = RetryConfig{}) const;
};

// ---- Queue Mediator ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessorCoroutine
// The mediator observes system state (online/offline, syncing) and the queue,
// then decides the next action.

// Original Kotlin: canReachServer AtomicBoolean + waitForNetwork
struct QueueMediatorState {
    bool isOnline = false;           // can reach homeserver
    bool isSyncing = false;          // initial sync in progress
    int eventsPending = 0;           // count of UNSENT events
    int eventsSending = 0;           // count of SENDING events
    int eventsFailed = 0;            // count of FAILED events
};

// Original Kotlin: executeTask / waitForNetwork / session lifecycle decision tree
enum class MediatorAction {
    SEND_PENDING,      // send the next pending event now
    RETRY_FAILED,      // retry a previously failed event
    WAIT               // nothing to do OR waiting for network/sync
};

// Decision tree, matching the Kotlin logic:
//   1. If offline → WAIT
//   2. If syncing → WAIT
//   3. If events pending → SEND_PENDING
//   4. If events failed → RETRY_FAILED
//   5. Otherwise → WAIT
MediatorAction decideNextAction(const QueueMediatorState& state);

// ---- Local Echo Factory ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.LocalEchoEventFactory
//              org.matrix.android.sdk.api.session.events.model.LocalEcho
//              org.matrix.android.sdk.internal.session.room.send.LocalEchoRepository

// Original Kotlin: result of LocalEchoEventFactory.createEvent()
// Holds the minimal metadata needed to identify and track a local echo.
struct LocalEchoInfo {
    std::string eventId;           // local event ID (same as transactionId initially)
    std::string transactionId;     // unsigned.transaction_id for dedup on sync
    int64_t timestamp = 0;         // origin_server_ts
    std::string senderId;
    std::string roomId;
};

// Original Kotlin: LocalEchoEventFactory.createEvent() + LocalEcho.createLocalEchoId()
// Create a local echo event JSON.
// This is the event displayed optimistically in the timeline before server confirms.
// Returns a JSON string representing the event with local ID and transaction_id.
std::string createLocalEcho(
    const std::string& roomId,
    const std::string& eventType,
    const std::string& contentJson,
    const std::string& senderId
);

// Original Kotlin: When sync returns an event whose unsigned.transaction_id
// matches a local echo's eventId, the local echo is replaced/merged with the server event.
// This function merges the two: server event body takes precedence,
// but local transient data (like send state) is preserved.
// Returns the merged event JSON.
std::string replaceLocalEcho(
    const std::string& localEchoJson,
    const std::string& serverEventJson
);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_RETRY_HPP
