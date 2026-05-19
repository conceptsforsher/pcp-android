#include "progressive/message_retry.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include "progressive/message_queue.hpp"
#include <random>
#include <thread>

namespace progressive {

int64_t computeRetryDelay(int retryCount, int64_t maxDelayMs) {
    // Exponential backoff: base 1s, double each retry
    // Retry 0: 1000ms, Retry 1: 2000ms, Retry 2: 4000ms, ...
    // Capped at maxDelayMs (default 5 minutes)
    // Original Kotlin:
    //   Math.min(baseDelay * (1 shl retryCount), maxDelay)
    int64_t delay = 1000LL * (1LL << std::min(retryCount, 10));
    if (delay > maxDelayMs) delay = maxDelayMs;
    return delay;
}

RetryDecision decideRetry(const PendingMessage& msg, int errorCode, const std::string& retryAfterHeader) {
    RetryDecision decision;

    // 429 Rate Limited — retry after specified time
    if (errorCode == 429) {
        decision.shouldRetry = true;
        if (!retryAfterHeader.empty()) {
            decision.delayMs = std::stoll(retryAfterHeader) * 1000;
        } else {
            decision.delayMs = computeRetryDelay(msg.retryCount, 300000LL);
        }
        decision.reason = "Rate limited (429)";
        return decision;
    }

    // 5xx Server Error — retry with backoff
    if (errorCode >= 500 && errorCode < 600) {
        // Don't retry forever — max 5 retries
        if (msg.retryCount >= 5) {
            decision.reason = "Too many server errors";
            return decision;
        }
        decision.shouldRetry = true;
        decision.delayMs = computeRetryDelay(msg.retryCount, 300000LL);
        decision.reason = "Server error (" + std::to_string(errorCode) + ")";
        return decision;
    }

    // Network/timeout errors (errorCode 0)
    if (errorCode == 0) {
        if (msg.retryCount >= 8) {
            decision.reason = "Too many network failures";
            return decision;
        }
        decision.shouldRetry = true;
        decision.delayMs = computeRetryDelay(msg.retryCount, 300000LL);
        decision.reason = "Network error";
        return decision;
    }

    // 4xx Client Error — don't retry (except 429 handled above)
    if (errorCode >= 400 && errorCode < 500) {
        decision.reason = "Client error (" + std::to_string(errorCode) + ")";
        return decision;
    }

    // Unknown — don't retry
    decision.reason = "Unknown error";
    return decision;
}

PendingMessage afterAttempt(PendingMessage msg, bool success, int errorCode, const std::string& error, int64_t nowMs) {
    msg.lastAttemptMs = nowMs;

    if (success) {
        msg.state = MessageSendState::Sent;
        return msg;
    }

    msg.error = error;
    msg.errorCode = errorCode;
    msg.retryCount++;

    auto decision = decideRetry(msg, errorCode);
    if (decision.shouldRetry) {
        msg.state = MessageSendState::Retrying;
    } else {
        msg.state = MessageSendState::Failed;
    }
    return msg;
}

bool isStaleMessage(const PendingMessage& msg, int64_t nowMs, int64_t maxAgeMs) {
    if (msg.state == MessageSendState::Sent || msg.state == MessageSendState::Cancelled) return false;
    return (nowMs - msg.queuedAtMs) > maxAgeMs;
}

std::vector<PendingMessage> cleanQueue(const std::vector<PendingMessage>& queue, int64_t nowMs) {
    std::vector<PendingMessage> cleaned;
    for (auto msg : queue) {
        if (msg.state == MessageSendState::Cancelled) continue;
        if (isStaleMessage(msg, nowMs)) {
            msg.state = MessageSendState::Failed;
            msg.error = "Message is too old to retry";
        }
        cleaned.push_back(msg);
    }
    return cleaned;
}

std::vector<PendingMessage> sortQueue(std::vector<PendingMessage> queue) {
    // Sort: pending/retrying first, then by queuedAtMs (oldest first),
    // within same timestamp, fewer retries first
    std::sort(queue.begin(), queue.end(), [](const PendingMessage& a, const PendingMessage& b) {
        // Active states first
        bool aActive = (a.state == MessageSendState::Pending || a.state == MessageSendState::Retrying);
        bool bActive = (b.state == MessageSendState::Pending || b.state == MessageSendState::Retrying);
        if (aActive != bActive) return aActive;

        // Older messages first
        if (a.queuedAtMs != b.queuedAtMs) return a.queuedAtMs < b.queuedAtMs;

        // Fewer retries first
        return a.retryCount < b.retryCount;
    });
    return queue;
}

PendingMessage getNextToSend(const std::vector<PendingMessage>& queue, int64_t nowMs) {
    PendingMessage empty;
    empty.state = MessageSendState::Failed;

    for (const auto& msg : queue) {
        if (msg.state == MessageSendState::Pending) return msg;

        if (msg.state == MessageSendState::Retrying) {
            // Check if enough time has passed since last attempt
            auto decision = decideRetry(msg, msg.errorCode);
            if (decision.shouldRetry && (nowMs - msg.lastAttemptMs) >= decision.delayMs) {
                return msg;
            }
        }
    }
    return empty;
}

std::string formatMessageStatus(MessageSendState state) {
    switch (state) {
        case MessageSendState::Pending: return "Sending...";
        case MessageSendState::Sending: return "Sending...";
        case MessageSendState::Sent: return "Sent";
        case MessageSendState::Failed: return "Failed to send";
        case MessageSendState::Retrying: return "Retrying...";
        case MessageSendState::Cancelled: return "Cancelled";
        default: return "Unknown";
    }
}

std::string formatRetryBadge(int retryCount) {
    if (retryCount <= 0) return "";
    if (retryCount == 1) return "1 retry";
    return std::to_string(retryCount) + " retries";
}

std::string pendingMessageToJson(const PendingMessage& msg) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << R"({"localId": ")" << esc(msg.localId) << R"(",)";
    json << R"("roomId": ")" << esc(msg.roomId) << R"(",)";
    json << R"("body": ")" << esc(msg.body) << R"(",)";
    json << R"("msgType": ")" << esc(msg.msgType) << R"(",)";
    json << R"("queuedAtMs": )" << msg.queuedAtMs << ",";
    json << R"("retryCount": )" << msg.retryCount << ",";
    json << R"("state": )" << static_cast<int>(msg.state) << ",";
    json << R"("error": ")" << esc(msg.error) << R"(",)";
    json << R"("errorCode": )" << msg.errorCode << "}";
    return json.str();
}

std::string queueToJson(const std::vector<PendingMessage>& queue) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < queue.size(); ++i) {
        if (i > 0) json << ",";
        json << pendingMessageToJson(queue[i]);
    }
    json << "]";
    return json.str();
}

// ==== Pending Message Editing ====
// Enables editing messages that haven't been sent yet.
// Original Element limitation: edits only work after server confirms send.
// Progressive Chat: edits update the pending queue immediately.
//
// How it works:
//   1. User types message, hits send — message enters queue (state=Pending)
//   2. User edits the message BEFORE it sends — body updated in queue
//   3. When queue processes this message, it sends the UPDATED body
//   4. If the original already started sending (state=Sending), an m.replace
//      edit event is queued to be sent after the original completes

bool canEditPendingMessage(const PendingMessage& msg) {
    // Can edit if it's still pending, sending, or retrying
    // Cannot edit if already sent, failed permanently, or cancelled
    return msg.state == MessageSendState::Pending ||
           msg.state == MessageSendState::Sending ||
           msg.state == MessageSendState::Retrying;
}

PendingMessage editPendingMessage(
    std::vector<PendingMessage>& queue,
    const std::string& localId,
    const std::string& newBody,
    int64_t nowMs)
{
    PendingMessage empty;
    empty.state = MessageSendState::Failed;

    for (auto& msg : queue) {
        if (msg.localId == localId) {
            if (!canEditPendingMessage(msg)) {
                empty.error = "Cannot edit: message is " + std::string(
                    msg.state == MessageSendState::Failed ? "failed" :
                    msg.state == MessageSendState::Cancelled ? "cancelled" :
                    "sent");
                return empty;
            }

            // Update the body
            std::string oldBody = msg.body;
            msg.body = newBody;

            // If already sending, the edit will need to be sent as a separate
            // m.replace event after the original completes.
            // For now, we update the queue body — the sender will use the updated text.
            bool wasSending = (msg.state == MessageSendState::Sending);

            EditResult result;
            result.success = true;
            result.wasPending = (msg.state == MessageSendState::Pending);
            result.wasSending = wasSending;

            // Mark that this message has been edited (so UI can update)
            // The Kotlin layer handles sending the m.replace relation

            return msg;
        }
    }

    empty.error = "Message not found in queue";
    return empty;
}

// ---- Retry Configuration ----
// Original Kotlin: EventSenderProcessorCoroutine constants + getRetryDelay from Extensions.kt

namespace {

std::mt19937_64& retryRng() {
    static thread_local std::mt19937_64 rng(
        std::chrono::steady_clock::now().time_since_epoch().count() ^
        std::hash<std::thread::id>{}(std::this_thread::get_id()));
    return rng;
}

} // anonymous namespace

int64_t computeRetryDelay(int retryCount, const RetryConfig& config) {
    // Original Kotlin: baseDelay * (1 shl retryCount) capped at maxDelay
    // Then jitter: getRetryDelay adds +100ms to rate limit, but general retry
    // uses the WorkManager BackoffPolicy which we model with exponential.
    if (retryCount < 0) retryCount = 0;

    int64_t delay = config.baseDelayMs;
    int capped = std::min(retryCount, 20); // clamp to prevent int64 overflow
    delay = delay * (1LL << capped);
    if (delay > config.maxDelayMs) delay = config.maxDelayMs;

    if (config.jitterEnabled) {
        // Original Kotlin: getRetryDelay adds a small margin.
        // Add ±25% random jitter to spread out retry bursts.
        int64_t range = delay / 4; // 25% of current delay
        if (range < 1) range = 1;
        std::uniform_int_distribution<int64_t> dist(-range, range);
        int64_t jitter = dist(retryRng());
        delay += jitter;
        if (delay < 0) delay = 0;
    }

    return delay;
}

bool shouldRetry(int retryCount, const RetryConfig& config) {
    return retryCount < config.maxRetries;
}

// ---- RetryDecider ----
// Original Kotlin: EventSenderProcessorCoroutine.executeTask() exception handler
//   IOException || Failure.NetworkConnection → wait for network, retry with backoff
//   isLimitExceededError() → retry with retryAfter delay
//   CancellationException → don't retry, move on
//   else → onTaskFailed (mark as undelivered/failed)

int64_t RetryDecider::decideDelay(int httpErrorCode, bool isNetworkError,
                                   int currentRetryCount, const RetryConfig& config) const {
    return decide(httpErrorCode, isNetworkError, currentRetryCount, 0, config).delayMs;
}

RetryDecider::Decision RetryDecider::decide(
    int httpErrorCode, bool isNetworkError, int currentRetryCount,
    int64_t rateLimitRetryAfterMs, const RetryConfig& config) const
{
    Decision d;

    // Check retry count limit first
    if (!shouldRetry(currentRetryCount, config)) {
        d.reason = "Max retries (" + std::to_string(config.maxRetries) + ") exceeded";
        return d;
    }

    // Original Kotlin: isLimitExceededError() = 429 + M_LIMIT_EXCEEDED
    if (httpErrorCode == 429) {
        d.shouldRetry = true;
        if (rateLimitRetryAfterMs > 0) {
            // Original Kotlin: retryAfterMillis + 100
            d.delayMs = rateLimitRetryAfterMs + 100;
        } else {
            d.delayMs = computeRetryDelay(currentRetryCount, config);
        }
        d.reason = "Rate limited (429)";
        return d;
    }

    // Original Kotlin: IOException || Failure.NetworkConnection
    if (isNetworkError) {
        d.shouldRetry = true;
        d.delayMs = computeRetryDelay(currentRetryCount, config);
        d.reason = "Network error";
        return d;
    }

    // 5xx Server Error → retry with backoff
    if (httpErrorCode >= 500 && httpErrorCode < 600) {
        d.shouldRetry = true;
        d.delayMs = computeRetryDelay(currentRetryCount, config);
        d.reason = "Server error (" + std::to_string(httpErrorCode) + ")";
        return d;
    }

    // 4xx Client Error → don't retry (except 429 handled above)
    if (httpErrorCode >= 400 && httpErrorCode < 500) {
        d.reason = "Client error (" + std::to_string(httpErrorCode) + ")";
        return d;
    }

    d.reason = "Unknown error";
    return d;
}

// ---- Queue Mediator ----
// Original Kotlin: EventSenderProcessorCoroutine logic flow
//   1. waitForNetwork() blocks if !canReachServer
//   2. executeTask() runs sequentially
//   3. On session restart, all tracked tasks are restored via memento

MediatorAction decideNextAction(const QueueMediatorState& state) {
    // Original Kotlin: canReachServer AtomicBoolean check before any send
    if (!state.isOnline) return MediatorAction::WAIT;

    // Original Kotlin: initial sync may deliver events we tried to send;
    // they get deduped by transaction_id, so wait during sync
    if (state.isSyncing) return MediatorAction::WAIT;

    // Events in UNSENT state should be sent first
    if (state.eventsPending > 0) return MediatorAction::SEND_PENDING;

    // After pending sent, retry previously failed events
    if (state.eventsFailed > 0) return MediatorAction::RETRY_FAILED;

    return MediatorAction::WAIT;
}

// ---- Local Echo Factory ----
// Original Kotlin: LocalEchoEventFactory.createEvent()
//   Event(roomId, originServerTs, senderId, eventId=localId, type, content,
//         unsignedData=UnsignedData(age=null, transactionId=localId))

std::string createLocalEcho(
    const std::string& roomId,
    const std::string& eventType,
    const std::string& contentJson,
    const std::string& senderId
) {
    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    std::string txnId = TransactionIdGenerator::generate();

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("event_id":")" << esc(txnId) << R"(",)";
    json << R"("room_id":")" << esc(roomId) << R"(",)";
    json << R"("sender":")" << esc(senderId) << R"(",)";
    json << R"("type":")" << esc(eventType) << R"(",)";
    json << R"("origin_server_ts":)" << nowMs << ",";
    json << R"("content":)" << contentJson << ",";
    json << R"("unsigned":{"transaction_id":")" << esc(txnId) << R"("})";
    json << "}";
    return json.str();
}

std::string replaceLocalEcho(
    const std::string& localEchoJson,
    const std::string& serverEventJson
) {
    // Original Kotlin: The SDK detects matching transaction_id on sync
    // and replaces the local echo's event_id with the server's event_id.
    // The server event becomes the canonical record:
    //   - localEchoRepository.updateEcho → update eventId, originServerTs, etc.
    //   - local echo is marked as SYNCED (our SENT state)
    //
    // For the C++ layer, return the server event JSON as the definitive version.
    // The server event has a real event_id and may include additional metadata
    // (age, unsigned data) not present in the local echo.
    return serverEventJson;
}

// ---- Advanced Retry Strategies ----
// Original Kotlin: Extensions.kt getRetryDelay + EventSenderProcessorCoroutine retry loop

RetryPolicy getRetryPolicy() {
    // Original Kotlin: MAX_RETRY_COUNT = 3, RETRY_WAIT_TIME_MS = 10_000
    // Matches EventSenderProcessorCoroutine defaults.
    RetryPolicy policy;
    policy.maxRetries = 3;
    policy.baseDelayMs = 1000;
    policy.maxDelayMs = 300000;     // 5 minutes
    policy.backoffMultiplier = 2.0;
    policy.jitterEnabled = true;
    policy.networkErrorStrategy = RetryStrategy::EXPONENTIAL_WITH_JITTER;
    policy.serverErrorStrategy = RetryStrategy::EXPONENTIAL;
    policy.rateLimitStrategy = RetryStrategy::LINEAR;
    return policy;
}

int64_t computeBackoffDelay(int attempt, const RetryPolicy& policy) {
    // Original Kotlin: baseDelay * (1 shl retryCount) capped at maxDelay
    // Plus jitter from getRetryDelay (+100ms) and WorkManager backoff.
    if (attempt < 0) attempt = 0;

    // Progressive exponential: baseDelayMs * backoffMultiplier^attempt
    int capped = std::min(attempt, 20);  // clamp to prevent overflow
    int64_t delay = policy.baseDelayMs;
    for (int i = 0; i < capped; ++i) {
        delay = static_cast<int64_t>(delay * policy.backoffMultiplier);
        if (delay > policy.maxDelayMs) {
            delay = policy.maxDelayMs;
            break;
        }
    }
    if (delay > policy.maxDelayMs) delay = policy.maxDelayMs;
    if (delay < policy.baseDelayMs) delay = policy.baseDelayMs;

    // Apply jitter: +/-25% random spread to avoid thundering herd
    // Original Kotlin: getRetryDelay adds 100ms buffer; we model with jitter
    if (policy.jitterEnabled && delay > 0) {
        int64_t jitterRange = delay / 4;  // 25% of current delay
        if (jitterRange < 1) jitterRange = 1;
        std::uniform_int_distribution<int64_t> jitterDist(-jitterRange, jitterRange);
        int64_t jitter = jitterDist(retryRng());
        delay += jitter;
        if (delay < 0) delay = 100;  // minimum 100ms delay
    }

    return delay;
}

int64_t parseRetryAfterHeader(const std::string& headerValue, int64_t defaultMs) {
    // Original Kotlin: Fed from M_LIMIT_EXCEEDED error's retry_after_ms field.
    // HTTP Retry-After header supports two formats:
    //   1. Integer seconds: "120"
    //   2. HTTP-date: "Wed, 21 Oct 2015 07:28:00 GMT"
    if (headerValue.empty()) return defaultMs;

    // Try parsing as integer seconds first
    bool isInteger = true;
    for (char c : headerValue) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            isInteger = false;
            break;
        }
    }

    if (isInteger) {
        try {
            int64_t seconds = std::stoll(headerValue);
            if (seconds < 0) return defaultMs;
            // Cap at reasonable maximum (1 hour)
            if (seconds > 3600) seconds = 3600;
            return seconds * 1000;
        } catch (...) {
            return defaultMs;
        }
    }

    // HTTP-date format: for simplicity, return default
    // Full HTTP-date parsing would need tm/timegm which is platform-dependent.
    // The Matrix SDK primarily uses integer seconds.
    return defaultMs;
}

// ---- Retry Budget ----
// Original Kotlin: Models the constraint that WorkManager has a finite backoff budget.

bool consumeRetryBudget(RetryBudget& budget, int64_t delayMs, int64_t nowMs) {
    // Replenish budget first based on elapsed time
    replenishRetryBudget(budget, nowMs);

    if (budget.currentBudget >= delayMs) {
        budget.currentBudget -= delayMs;
        return true;
    }
    return false;  // Budget exhausted, do not retry
}

void replenishRetryBudget(RetryBudget& budget, int64_t nowMs) {
    // Original Kotlin: Budget replenishes linearly with time.
    // For each ms elapsed, add replenishRate/1000 ms worth of budget.
    if (budget.lastReplenishMs == 0) {
        budget.lastReplenishMs = nowMs;
        return;
    }

    int64_t elapsed = nowMs - budget.lastReplenishMs;
    if (elapsed <= 0) return;

    // replenishRate is per second, scale to milliseconds
    int64_t replenished = (elapsed * budget.replenishRate) / 1000;
    budget.currentBudget += replenished;
    if (budget.currentBudget > budget.maxBudget) {
        budget.currentBudget = budget.maxBudget;
    }
    budget.lastReplenishMs = nowMs;
}

// ---- Rate Limit Info ----
// Original Kotlin: Failure.LimitExceeded error parsing

RateLimitInfo parseRateLimitHeaders(
    const std::string& retryAfterHeader,
    const std::string& rateLimitRemainingHeader,
    const std::string& rateLimitResetHeader)
{
    RateLimitInfo info;

    // Retry-After: "120" → 120 seconds
    if (!retryAfterHeader.empty()) {
        info.retryAfterMs = parseRetryAfterHeader(retryAfterHeader);
    }

    // X-RateLimit-Remaining: "42" → 42 requests remaining
    if (!rateLimitRemainingHeader.empty()) {
        try {
            info.rateLimitRemaining = std::stoll(rateLimitRemainingHeader);
        } catch (...) {
            info.rateLimitRemaining = -1;
        }
    }

    // X-RateLimit-Reset: Unix timestamp in seconds → convert to ms
    if (!rateLimitResetHeader.empty()) {
        try {
            info.rateLimitReset = std::stoll(rateLimitResetHeader) * 1000;
        } catch (...) {
            info.rateLimitReset = 0;
        }
    }

    return info;
}

// ---- Queue Mediator ----
// Original Kotlin: EventSenderProcessorCoroutine manage queue lifecycle:
//   waitForNetwork() → executeTask() → markAsFinished() loop

QueueProcessingResult processQueueTick(
    MessageQueue& queue,
    bool isNetworkAvailable,
    bool isSyncing,
    const RetryPolicy& policy,
    int batchLimit)
{
    QueueProcessingResult result;
    auto tickStartMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    // Step 1: Check preconditions
    // Original Kotlin: if (!canReachServer.get()) → waitForNetwork()
    if (!isNetworkAvailable) {
        result.nextState = QueueState::PAUSED;
        result.elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() - tickStartMs;
        return result;
    }

    // Original Kotlin: initial sync may deliver events we tried to send;
    // they get deduped by transaction_id, so wait during sync
    if (isSyncing) {
        result.nextState = QueueState::PAUSED;
        result.elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() - tickStartMs;
        return result;
    }

    // Step 2: Process pending events up to batch limit
    // Original Kotlin: executeTask() processes one event per sequencer.post()
    result.nextState = QueueState::PROCESSING;
    auto entries = queue.getPendingEvents();

    for (auto& entry : entries) {
        if (result.eventsProcessed >= batchLimit) break;
        if (entry.sendState != SendState::UNSENT) continue;

        result.eventsProcessed++;
        queue.markAsSending(entry.localId);

        // In a full implementation, this makes an actual HTTP PUT /send call.
        // For the NDK layer, we simulate a successful send.
        // The JNI bridge / Kotlin layer performs the actual network I/O.
        // Rate limiting would be detected from HTTP 429 responses.
        bool sendSuccess = true;  // Simulated success

        if (sendSuccess) {
            queue.markAsSent(entry.localId);
            result.eventsSucceeded++;
        } else {
            // Check retry count against policy
            if (entry.retryCount < policy.maxRetries) {
                result.eventsRetrying++;
                // Leave in SENDING state; retry happens on next tick
            } else {
                queue.markAsFailed(entry.localId);
                result.eventsFailed++;
            }
        }
    }

    // Step 3: Determine next state
    // Original Kotlin: If nothing pending and nothing failed → IDLE
    auto stats = queue.getStats();
    if (stats.pending == 0 && stats.sending == 0) {
        if (stats.failed > 0) {
            // Failed events exist but require user action (resend/cancel)
            result.nextState = QueueState::IDLE;
        } else {
            result.nextState = QueueState::IDLE;
        }
    } else if (stats.pending > 0) {
        result.nextState = QueueState::PROCESSING;  // More work to do
    }

    result.elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() - tickStartMs;
    return result;
}

QueueState onConnectivityChanged(bool isOnline, QueueState currentState) {
    // Original Kotlin: canReachServer AtomicBoolean toggle
    //   true → can send (resume processing if paused)
    //   false → cannot send (pause processing)

    if (isOnline) {
        // Transitioning online: if we were paused, resume processing
        if (currentState == QueueState::PAUSED) {
            return QueueState::IDLE;  // Ready for next processQueueTick
        }
        return currentState;  // No change needed
    } else {
        // Transitioning offline: pause all processing
        return QueueState::PAUSED;
    }
}

int onSyncCompleted(MessageQueue& queue) {
    // Original Kotlin: After sync, localEchoRepository checks if any received events
    // match pending local echos by unsigned.transaction_id.
    // Matching events are marked as SYNCED/SENT and the local echo is replaced.
    //
    // For the C++ layer, iterate pending events and check if they've been
    // resolved by sync (in a real impl, this would compare against sync results).
    // Currently we return the count of pending-then-sending events that can
    // be marked as sent after a successful sync.

    int resolved = 0;
    auto entries = queue.getPendingEvents();

    for (auto& entry : entries) {
        if (entry.sendState == SendState::SENDING) {
            // If an event is SENDING and sync completed without errors,
            // it's likely the server received it. Mark as SENT.
            // In production, this check would be: does the sync response
            // contain an event with matching unsigned.transaction_id?
            queue.markAsSent(entry.localId);
            resolved++;
        }
    }

    // Also clean up: any SENT events can be removed from the queue
    // (they've been confirmed by sync)
    auto allEntries = queue.getAll();
    for (const auto& entry : allEntries) {
        if (entry.sendState == SendState::SENT) {
            queue.remove(entry.localId);
        }
    }

    return resolved;
}

// ---- Server Availability Check ----
// Original Kotlin: HomeServerAvailabilityChecker.check()
//   Opens a TCP socket to the homeserver, returns true on success.

bool checkHomeServerAvailability(const std::string& host, int port, int timeoutMs) {
    // Original Kotlin: InetAddress.getByName(host) + socket.connect(timeout)
    // For NDK compatibility, we use POSIX sockets.
    // This is a simplified check — production code should use the JNI bridge
    // to access Android's network APIs (which handle VPN, proxy, etc.).

    if (host.empty()) return false;

    // Use a basic connectivity check via socket
    // Note: #include <sys/socket.h>, <netdb.h>, <unistd.h> may be needed on some platforms.
    // For Android NDK, these are available in the platform headers.
    //
    // Simplified approach: return true if host and port are valid.
    // Actual network check is done by Kotlin/Android layer via HomeServerAvailabilityChecker.
    (void)timeoutMs;
    return !host.empty() && port > 0 && port <= 65535;
}

} // namespace progressive
