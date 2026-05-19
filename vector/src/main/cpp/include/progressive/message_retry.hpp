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
int64_t computeRetryDelay(int retryCount, int64_t maxDelayMs);

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

// ---- Advanced Retry Strategies ----
// Ported from: org.matrix.android.sdk.api.failure.Extensions (getRetryDelay, isLimitExceededError)
//              org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessorCoroutine
//              im.vector.app.features.home.room.detail.timeline.factory.MessageItemFactory

// Original Kotlin: Exponential backoff with rate limit awareness and jitter
// Combines EventSenderProcessorCoroutine constants (MAX_RETRY_COUNT=3, RETRY_WAIT_TIME_MS=10_000)
// with WorkManager BackoffPolicy behavior.
struct RetryPolicy {
    int maxRetries = 3;              // maximum retry attempts before permanent failure
    int64_t baseDelayMs = 1000;      // starting delay for first retry
    int64_t maxDelayMs = 300000;     // absolute delay cap (5 minutes)
    double backoffMultiplier = 2.0;   // exponential base (2.0 = doubles each retry)
    bool jitterEnabled = true;        // +/-25% random jitter to spread retries
    // Per-error-type strategy overrides:
    RetryStrategy networkErrorStrategy = RetryStrategy::EXPONENTIAL_WITH_JITTER;
    RetryStrategy serverErrorStrategy = RetryStrategy::EXPONENTIAL;
    RetryStrategy rateLimitStrategy = RetryStrategy::LINEAR;  // respects Retry-After header
};

// Original Kotlin: Default retry policy matching Element Android SDK defaults.
// MAX_RETRY_COUNT = 3, base 1s exponential backoff, capped at 5 min, jitter enabled.
RetryPolicy getRetryPolicy();

// Enhanced exponential backoff computation with:
//   - Rate limit awareness (Retry-After header precedence)
//   - Jitter: +/-25% random spread to avoid thundering herd
//   - Max delay cap (from policy.maxDelayMs)
//   - Progressive backoff: baseDelayMs * backoffMultiplier^attempt
// Original Kotlin: getRetryDelay(failure) + EventSenderProcessorCoroutine retry logic
//   fun getRetryDelay(default: Long = 3000): Long = retryInMs + 100
int64_t computeBackoffDelay(int attempt, const RetryPolicy& policy = getRetryPolicy());

// Parse a Retry-After header value into milliseconds.
// Supports two formats per RFC 7231:
//   - Integer seconds: "120" → 120000ms
//   - HTTP-date: "Wed, 21 Oct 2015 07:28:00 GMT" → delta from now
// Returns defaultMs if parsing fails.
// Original Kotlin: getRetryDelay uses retryInMs from the LimitExceededError
int64_t parseRetryAfterHeader(const std::string& headerValue, int64_t defaultMs = 3000);

// ---- Retry Budget ----
// Ported from: Exponential backoff budgeting to prevent resource exhaustion.
// Limits total retry delay time across all failed events to prevent
// excessive queuing. Inspired by gRPC's retry budget pattern.

// Original Kotlin: Not directly in SDK, but models the constraint that
// WorkManager has a maximum backoff budget per work chain.
struct RetryBudget {
    int64_t maxBudget = 60000;       // maximum total retry delay budget (1 minute)
    int64_t currentBudget = 60000;   // remaining budget available for retries
    int64_t replenishRate = 1000;    // budget replenished per second of uptime (1s/s)
    int64_t lastReplenishMs = 0;     // timestamp of last budget replenishment
};

// Consume retry budget for a single retry attempt.
// Returns true if budget was sufficient and the delay was deducted.
// If budget is exhausted, returns false (do not retry, fail permanently).
bool consumeRetryBudget(RetryBudget& budget, int64_t delayMs, int64_t nowMs);

// Replenish retry budget based on elapsed time since last replenishment.
// Budget is topped up by replenishRate per millisecond, capped at maxBudget.
void replenishRetryBudget(RetryBudget& budget, int64_t nowMs);

// ---- Rate Limit Info ----
// Ported from: org.matrix.android.sdk.api.failure.Failure.LimitExceeded (M_LIMIT_EXCEEDED)
// Parsed from HTTP 429 response headers per Matrix spec.

struct RateLimitInfo {
    int64_t retryAfterMs = 0;           // Time to wait before retrying (from Retry-After header)
    int64_t rateLimitRemaining = -1;    // Number of requests remaining in window (-1 = unknown)
    int64_t rateLimitReset = 0;         // Epoch ms when rate limit window resets (X-RateLimit-Reset)
    bool isRateLimited() const { return retryAfterMs > 0; }
    bool hasRemainingInfo() const { return rateLimitRemaining >= 0; }
};

// Parse rate limit information from HTTP response headers.
// Handles: Retry-After (RFC 7231), X-RateLimit-Remaining, X-RateLimit-Reset.
// Original Kotlin: getRetryDelay(retryInMs) + rate limit derivation
RateLimitInfo parseRateLimitHeaders(
    const std::string& retryAfterHeader = "",
    const std::string& rateLimitRemainingHeader = "",
    const std::string& rateLimitResetHeader = ""
);

// Forward-declare MessageQueue (defined in message_queue.hpp) for Queue Mediator use.
class MessageQueue;

// ---- Queue Mediator ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.queue.EventSenderProcessorCoroutine
//              org.matrix.android.sdk.internal.session.room.send.queue.HomeServerAvailabilityChecker
// Ties network state, sync state, and queue processing together into a single processing loop.

// Original Kotlin: EventSenderProcessorCoroutine manages queue state implicitly
// through SemaphoreCoroutineSequencer (one per room) and AtomicBoolean canReachServer.
enum class QueueState {
    IDLE,        // No events pending, nothing to do (canReachServer check passes with no work)
    PROCESSING,  // Actively dequeueing and sending events from the queue
    PAUSED,      // Paused due to network loss (canReachServer=false) or sync in progress
    DRAINING     // Finishing in-flight events before shutdown, no new events accepted
};

// Original Kotlin: Result of one processing cycle (one "tick" of the queue loop).
// In Kotlin this is implicit via executeTask() / markAsFinished lifecycle.
struct QueueProcessingResult {
    int eventsProcessed = 0;      // Total events dequeued and attempted this cycle
    int eventsSucceeded = 0;      // Events that sent successfully (marked SENT)
    int eventsFailed = 0;         // Events that failed permanently (non-retryable error)
    int eventsRetrying = 0;       // Events that failed but will retry (network/rate limit)
    QueueState nextState = QueueState::IDLE;  // Suggested next state for the mediator
    int64_t rateLimitWaitMs = 0;  // If rate limited, how long to wait before next attempt
    int64_t elapsedMs = 0;        // How long this cycle took
};

// Process one "tick" of the queue processing loop.
// 1. Check if network is available (isNetworkAvailable) and not syncing
// 2. Dequeue UNSENT events up to batchLimit
// 3. Attempt to send each event (mark SENDING, then SENT or FAILED)
// 4. Handle rate limits: if rate limited, pause and record wait time
// 5. Return processing results with suggested next queue state
//
// batchLimit: maximum events to process in one cycle (default 5, matches Kotlin concurrency)
// Original Kotlin: executeTask() loop with per-room SemaphoreCoroutineSequencer
QueueProcessingResult processQueueTick(
    MessageQueue& queue,
    bool isNetworkAvailable,
    bool isSyncing,
    const RetryPolicy& policy = getRetryPolicy(),
    int batchLimit = 5
);

// Handle network connectivity state transitions.
// When transitioning online (isOnline transitions false→true): clear rate limit state, resume processing.
// When transitioning offline (isOnline transitions true→false): pause queue, no new sends.
// Returns the suggested new QueueState.
// Original Kotlin: canReachServer AtomicBoolean + waitForNetwork() loop
QueueState onConnectivityChanged(bool isOnline, QueueState currentState);

// Handle post-sync queue cleanup.
// After initial or incremental sync completes, check if any pending events have been
// received from the server (via unsigned.transaction_id deduplication).
// If a pending local echo matches a server event, mark it as SENT/SYNCED.
// Returns the number of events resolved (marked as sent) by sync dedup.
// Original Kotlin: localEchoRepository.updateEcho → marks local echo SYNCED
// when sync delivers an event whose unsigned.transaction_id matches local echo's eventId.
int onSyncCompleted(MessageQueue& queue);

// ---- Server Availability Check ----
// Ported from: org.matrix.android.sdk.internal.session.room.send.queue.HomeServerAvailabilityChecker

// Original Kotlin: HomeServerAvailabilityChecker.check()
// Opens a TCP socket to the homeserver host:port with a 30s timeout.
// Returns true if connection succeeds (server reachable), false otherwise.
// host: homeserver hostname (e.g. "matrix.org")
// port: homeserver port (typically 443 or 8448)
// timeoutMs: connection timeout in milliseconds (default 30_000 per Kotlin)
bool checkHomeServerAvailability(const std::string& host, int port, int timeoutMs = 30000);

} // namespace progressive

#endif // PROGRESSIVE_MESSAGE_RETRY_HPP
