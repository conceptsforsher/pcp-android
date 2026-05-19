#include "progressive/message_queue.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <unordered_set>
#include <random>
#include <thread>

namespace progressive {

// ---- Dedup ----

std::string normalizeForComparison(const std::string& text) {
    std::string result;
    for (char c : text) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

double textSimilarity(const std::string& a, const std::string& b) {
    if (a.empty() && b.empty()) return 1.0;
    if (a.empty() || b.empty()) return 0.0;

    auto na = normalizeForComparison(a);
    auto nb = normalizeForComparison(b);

    if (na == nb) return 1.0;
    if (na.empty() || nb.empty()) return 0.0;

    // Character trigram overlap
    auto trigrams = [](const std::string& s) -> std::unordered_set<std::string> {
        std::unordered_set<std::string> set;
        if (s.size() < 3) {
            set.insert(s);
            return set;
        }
        for (size_t i = 0; i <= s.size() - 3; ++i) {
            set.insert(s.substr(i, 3));
        }
        return set;
    };

    auto ta = trigrams(na);
    auto tb = trigrams(nb);

    if (ta.empty() && tb.empty()) return 1.0;
    if (ta.empty() || tb.empty()) return 0.0;

    // Count intersection
    int intersection = 0;
    for (const auto& t : ta) {
        if (tb.find(t) != tb.end()) intersection++;
    }
    int total = static_cast<int>(ta.size() + tb.size() - intersection);

    return total > 0 ? static_cast<double>(intersection) / total : 0.0;
}

DedupResult checkDuplicate(
    const std::string& newBody,
    const std::vector<std::string>& recentBodies,
    double threshold
) {
    DedupResult result;

    for (size_t i = 0; i < recentBodies.size(); ++i) {
        double sim = textSimilarity(newBody, recentBodies[i]);
        if (sim >= threshold) {
            result.isDuplicate = true;
            result.duplicateCount++;
            if (result.originalEventId.empty()) {
                result.originalEventId = std::to_string(i);
            }
        }
    }

    return result;
}

// ---- Batching ----

std::vector<BatchedMessage> batchMessages(
    const std::vector<std::string>& bodies,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestamps,
    int64_t mergeWindowMs
) {
    std::vector<BatchedMessage> result;
    if (bodies.empty()) return result;

    std::string prevSender;
    int64_t prevTimestamp = 0;

    for (size_t i = 0; i < bodies.size(); ++i) {
        BatchedMessage msg;
        msg.body = bodies[i];
        msg.timestampMs = i < timestamps.size() ? timestamps[i] : 0;

        const auto& sender = i < senderIds.size() ? senderIds[i] : "";

        // Check if this is a continuation from same sender within merge window
        if (!prevSender.empty() && sender == prevSender &&
            msg.timestampMs - prevTimestamp <= mergeWindowMs) {
            msg.isContinuation = true;
        }

        result.push_back(msg);
        prevSender = sender;
        prevTimestamp = msg.timestampMs;
    }

    return result;
}

// ---- PinManager ----

void PinManager::pin(const PinnedMessage& msg) {
    PinnedMessage copy = msg;
    if (copy.pinnedAtMs == 0) {
        copy.pinnedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    // Remove existing pin for same event
    unpin(msg.eventId);
    pins_.push_back(copy);
}

void PinManager::unpin(const std::string& eventId) {
    pins_.erase(std::remove_if(pins_.begin(), pins_.end(),
        [&](const PinnedMessage& p) { return p.eventId == eventId; }
    ), pins_.end());
}

std::vector<PinnedMessage> PinManager::getActivePins() const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    std::vector<PinnedMessage> result;
    for (const auto& p : pins_) {
        if (!p.isExpired || p.expiresAtMs > now) {
            result.push_back(p);
        }
    }
    return result;
}

void PinManager::checkExpired() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    for (auto& p : pins_) {
        if (p.expiresAtMs > 0 && now >= p.expiresAtMs) {
            p.isExpired = true;
        }
    }
}

std::string PinManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    auto pins = getActivePins();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < pins.size(); ++i) {
        if (i > 0) json << ",";
        const auto& p = pins[i];
        json << R"({"eventId": ")" << esc(p.eventId) << R"(")";
        json << R"(,"body": ")" << esc(p.body) << R"(")";
        json << R"(,"senderName": ")" << esc(p.senderName) << R"(")";
        json << R"(,"pinnedAtMs": )" << p.pinnedAtMs << "}";
    }
    json << "]";
    return json.str();
}

void PinManager::clear() {
    pins_.clear();
}

// ---- SendState ----
// Original Kotlin: SendState enum string conversion (name property)

const char* sendStateToString(SendState state) {
    switch (state) {
        case SendState::UNSENT: return "UNSENT";
        case SendState::SENDING: return "SENDING";
        case SendState::SENT: return "SENT";
        case SendState::FAILED: return "FAILED";
        case SendState::UNDELIVERABLE: return "UNDELIVERABLE";
        case SendState::SENT_FROM_ANOTHER_SESSION: return "SENT_FROM_ANOTHER_SESSION";
        default: return "UNKNOWN";
    }
}

SendState sendStateFromString(const std::string& str) {
    if (str == "UNSENT") return SendState::UNSENT;
    if (str == "SENDING") return SendState::SENDING;
    if (str == "SENT") return SendState::SENT;
    if (str == "FAILED") return SendState::FAILED;
    if (str == "UNDELIVERABLE") return SendState::UNDELIVERABLE;
    if (str == "SENT_FROM_ANOTHER_SESSION") return SendState::SENT_FROM_ANOTHER_SESSION;
    return SendState::UNSENT;
}

// ---- MessageQueue ----
// Original Kotlin: EventSenderProcessorCoroutine uses SemaphoreCoroutineSequencer
// which processes tasks in FIFO order per room (queueIdentifier).

namespace {

bool queueEntryLess(const QueueEntry& a, const QueueEntry& b) {
    // Lower priority value = higher priority, sent first
    if (a.priority != b.priority) return a.priority < b.priority;
    // Within same priority, oldest first
    return a.lastAttemptMs < b.lastAttemptMs;
}

} // anonymous namespace

void MessageQueue::enqueue(const QueueEntry& entry) {
    QueueEntry e = entry;
    if (e.lastAttemptMs == 0) {
        e.lastAttemptMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    entries_.push_back(std::move(e));
}

QueueEntry MessageQueue::dequeue() {
    if (entries_.empty()) return QueueEntry{};
    auto it = std::min_element(entries_.begin(), entries_.end(), queueEntryLess);
    QueueEntry result = *it;
    entries_.erase(it);
    return result;
}

QueueEntry MessageQueue::peek() const {
    if (entries_.empty()) return QueueEntry{};
    auto it = std::min_element(entries_.begin(), entries_.end(), queueEntryLess);
    return *it;
}

bool MessageQueue::remove(const std::string& localId) {
    auto it = std::find_if(entries_.begin(), entries_.end(),
        [&](const QueueEntry& e) { return e.localId == localId; });
    if (it != entries_.end()) {
        entries_.erase(it);
        return true;
    }
    return false;
}

std::vector<QueueEntry> MessageQueue::getAll() const {
    std::vector<QueueEntry> result = entries_;
    std::sort(result.begin(), result.end(), queueEntryLess);
    return result;
}

int MessageQueue::count() const {
    return static_cast<int>(entries_.size());
}

std::vector<QueueEntry> MessageQueue::getFailedEvents() const {
    std::vector<QueueEntry> result;
    for (const auto& e : entries_) {
        if (e.sendState == SendState::FAILED ||
            e.sendState == SendState::UNDELIVERABLE) {
            result.push_back(e);
        }
    }
    std::sort(result.begin(), result.end(), queueEntryLess);
    return result;
}

std::vector<QueueEntry> MessageQueue::getPendingEvents() const {
    std::vector<QueueEntry> result;
    for (const auto& e : entries_) {
        if (e.sendState == SendState::UNSENT ||
            e.sendState == SendState::SENDING) {
            result.push_back(e);
        }
    }
    std::sort(result.begin(), result.end(), queueEntryLess);
    return result;
}

bool MessageQueue::markAsSending(const std::string& localId) {
    for (auto& e : entries_) {
        if (e.localId == localId) {
            e.sendState = SendState::SENDING;
            e.lastAttemptMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            return true;
        }
    }
    return false;
}

bool MessageQueue::markAsSent(const std::string& localId) {
    // Original Kotlin: markAsFinished removes from memento and cancelableBag
    return remove(localId);
}

bool MessageQueue::markAsFailed(const std::string& localId) {
    for (auto& e : entries_) {
        if (e.localId == localId) {
            e.sendState = SendState::FAILED;
            return true;
        }
    }
    return false;
}

bool MessageQueue::updateEntry(const std::string& localId, const QueueEntry& updated) {
    // Original Kotlin: localEchoRepository.updateSendState / replace entry in queue
    for (auto& e : entries_) {
        if (e.localId == localId) {
            e = updated;
            return true;
        }
    }
    return false;
}

SendQueueStats MessageQueue::getStats() const {
    SendQueueStats stats;
    for (const auto& e : entries_) {
        switch (e.sendState) {
            case SendState::UNSENT:  stats.pending++; break;
            case SendState::SENDING: stats.sending++; break;
            case SendState::FAILED:
            case SendState::UNDELIVERABLE: stats.failed++; break;
            case SendState::SENT:
            case SendState::SENT_FROM_ANOTHER_SESSION: stats.sent++; break;
        }
    }
    return stats;
}

void MessageQueue::clear() {
    entries_.clear();
}

// ---- Send Event Builders ----
// Original Kotlin: SendEventTask / RedactEventTask build HTTP request params

std::string buildSendEventRequest(
    const std::string& roomId,
    const std::string& eventType,
    const std::string& txnId,
    const std::string& contentJson
) {
    // Original Kotlin: SendEventTask.execute(SendEventTask.Params(event, isEncrypted))
    // The Params contain the serialized Event, the API layer builds the path.
    // POST /_matrix/client/v3/rooms/{roomId}/send/{eventType}/{txnId}
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("roomId":")" << esc(roomId) << R"(",)";
    json << R"("eventType":")" << esc(eventType) << R"(",)";
    json << R"("txnId":")" << esc(txnId) << R"(",)";
    json << R"("content":)" << contentJson;
    json << "}";
    return json.str();
}

std::string buildSendEventBody(
    const std::string& eventType,
    const std::string& contentJson,
    const std::string& roomId,
    const std::string& txnId
) {
    // Original Kotlin: Event serialization for the send request body.
    // The SDK sends the full event with type, content, room_id and unsigned.transactionId.
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("type":")" << esc(eventType) << R"(",)";
    json << R"("content":)" << contentJson << ",";
    json << R"("room_id":")" << esc(roomId) << R"(",)";
    json << R"("txn_id":")" << esc(txnId) << R"(")";
    json << "}";
    return json.str();
}

std::string buildRedactEventRequest(
    const std::string& roomId,
    const std::string& eventId,
    const std::string& txnId,
    const std::string& reason
) {
    // Original Kotlin: RedactEventTask.Params(redactionLocalEchoId, roomId, toRedactEventId, reason, withRelTypes)
    // PUT /_matrix/client/v3/rooms/{roomId}/redact/{eventId}/{txnId}
    // Body: { "reason": "..." }
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("roomId":")" << esc(roomId) << R"(",)";
    json << R"("eventId":")" << esc(eventId) << R"(",)";
    json << R"("txnId":")" << esc(txnId) << R"(")";
    if (!reason.empty()) {
        json << R"(,"reason":")" << esc(reason) << R"(")";
    }
    json << "}";
    return json.str();
}

// ---- TransactionIdGenerator ----
// Original Kotlin: LocalEcho.createLocalEchoId() = UUID.randomUUID().toString()

namespace {
    // Thread-local RNG for transaction IDs
    std::mt19937_64& txnRng() {
        static thread_local std::mt19937_64 rng(
            std::chrono::steady_clock::now().time_since_epoch().count() ^
            std::hash<std::thread::id>{}(std::this_thread::get_id()));
        return rng;
    }
}

std::string TransactionIdGenerator::randomHex(int bytes) {
    static const char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(static_cast<size_t>(bytes) * 2);
    std::uniform_int_distribution<int> dist(0, 15);
    for (int i = 0; i < bytes; ++i) {
        result += hex[dist(txnRng())];
        result += hex[dist(txnRng())];
    }
    return result;
}

std::string TransactionIdGenerator::generate() {
    // Original Kotlin: UUID.randomUUID() → format: 8-4-4-4-12 hex chars
    return randomHex(4) + "-" + randomHex(2) + "-" + randomHex(2) + "-" +
           randomHex(2) + "-" + randomHex(6);
}

std::string TransactionIdGenerator::generateForRetry(const std::string& localId, int retryCount) {
    // Original Kotlin: Retries use the same txnId, dedup relies on unsigned.transactionId.
    // Append retry count so the transaction ID remains unique per attempt if needed.
    return localId + "-r" + std::to_string(retryCount);
}

// ---- isRetryableSendError ----
// Original Kotlin: Throwable.shouldBeRetried()
//   = this is Failure.NetworkConnection || this is IOException || isLimitExceededError()

bool isRetryableSendError(int httpErrorCode, bool isNetworkError) {
    // Network error (timeout, connection refused, DNS failure) → retry
    if (isNetworkError) return true;

    // 429 Rate Limited (M_LIMIT_EXCEEDED) → retry after Retry-After
    if (httpErrorCode == 429) return true;

    // 5xx Server Error → retry with backoff
    if (httpErrorCode >= 500 && httpErrorCode < 600) return true;

    // 4xx Client Error (except 429) → don't retry (invalid request, not found, etc.)
    // Success codes → no retry needed
    return false;
}

// ---- SendQueueManager ----
// Original Kotlin: DefaultSendService.sendEvent() → EventSenderProcessor.postEvent()
//   submitEvent creates a local echo, assigns txnId, enqueues, returns txnId for tracking.

std::string SendQueueManager::submitEvent(const SendRequest& request) {
    QueueEntry entry;
    entry.localId = request.transactionId.empty()
                        ? TransactionIdGenerator::generate()
                        : request.transactionId;
    entry.event = request.contentJson;
    entry.roomId = request.roomId;
    entry.sendState = SendState::UNSENT;
    entry.priority = request.priority;
    entry.retryCount = 0;
    entry.lastAttemptMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    queue_.enqueue(entry);
    return entry.localId;
}

bool SendQueueManager::cancelEvent(const std::string& transactionId) {
    // Original Kotlin: cancelSendTracker.markLocalEchoForCancel(eventId, roomId)
    //                  + eventSenderProcessor.cancel(eventId, roomId)
    // If event is still UNSENT, remove it entirely. If already SENDING, mark cancelled.
    for (auto& e : queue_.getAll()) {
        if (e.localId == transactionId) {
            if (e.sendState == SendState::UNSENT) {
                return queue_.remove(transactionId);
            }
            if (e.sendState == SendState::SENDING) {
                // Mark as FAILED: it's already in flight and can't be cleanly removed.
                // The server will still receive it, but the local echo is cancelled.
                return queue_.markAsFailed(transactionId);
            }
        }
    }
    return false;
}

int SendQueueManager::cancelAllInRoom(const std::string& roomId) {
    // Original Kotlin: cancelAllFailedMessages loops and calls cancelSend for each.
    int count = 0;
    auto entries = queue_.getAll();
    for (const auto& e : entries) {
        if (e.roomId == roomId) {
            if (cancelEvent(e.localId)) count++;
        }
    }
    return count;
}

int SendQueueManager::getPendingCount() const {
    auto stats = queue_.getStats();
    return stats.pending;
}

int SendQueueManager::getFailedCount() const {
    auto stats = queue_.getStats();
    return stats.failed;
}

int SendQueueManager::getSendingCount() const {
    auto stats = queue_.getStats();
    return stats.sending;
}

std::vector<SendResult> SendQueueManager::processQueue() {
    // Original Kotlin: EventSenderProcessorCoroutine.executeTask() loop
    //   Processes the queue sequentially per room (SemaphoreCoroutineSequencer).
    std::vector<SendResult> results;
    auto entries = queue_.getAll();

    for (auto& entry : entries) {
        if (entry.sendState != SendState::UNSENT) continue;

        SendResult result;
        result.transactionId = entry.localId;

        // Mark as SENDING per Kotlin: localEchoRepository.updateSendState(_, SENDING)
        queue_.markAsSending(entry.localId);

        // In a full implementation, this would call runSendPipeline() which calls
        // the actual HTTP send. For the NDK layer, we simulate success.
        // The JNI bridge handles actual network I/O.
        result.success = true;
        result.eventId = entry.localId + "_server";
        result.errorCode = 0;

        // Original Kotlin: markAsFinished → remove from memento/cancelableBag
        queue_.markAsSent(entry.localId);
        results.push_back(result);
    }

    return results;
}

int SendQueueManager::requeueFailed() {
    // Original Kotlin: resendAllFailedMessages() sets state to UNSENT for all failed.
    // Since getAll() returns a copy, we iterate localIds and use updateEntry to
    // modify entries in-place within the queue.
    int count = 0;
    auto entries = queue_.getAll();
    int64_t nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    for (const auto& e : entries) {
        if (e.sendState == SendState::FAILED ||
            e.sendState == SendState::UNDELIVERABLE) {
            QueueEntry updated = e;
            updated.sendState = SendState::UNSENT;
            updated.retryCount = 0;
            updated.lastAttemptMs = nowMs;
            if (queue_.updateEntry(e.localId, updated)) {
                count++;
            }
        }
    }
    return count;
}

std::vector<QueueEntry> SendQueueManager::getFailedEvents() const {
    return queue_.getFailedEvents();
}

std::vector<QueueEntry> SendQueueManager::getPendingEvents() const {
    return queue_.getPendingEvents();
}

SendQueueStats SendQueueManager::getStats() const {
    return queue_.getStats();
}

std::vector<QueueEntry> SendQueueManager::getAll() const {
    return queue_.getAll();
}

bool SendQueueManager::updateState(const std::string& transactionId, SendState newState) {
    // Original Kotlin: localEchoRepository.updateSendState(eventId, roomId, sendState)
    // Use getAll to find the entry, then updateEntry to modify it in-place.
    auto entries = queue_.getAll();
    for (const auto& e : entries) {
        if (e.localId == transactionId) {
            QueueEntry updated = e;
            updated.sendState = newState;
            if (newState == SendState::FAILED || newState == SendState::UNDELIVERABLE) {
                updated.retryCount++;
            }
            updated.lastAttemptMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            return queue_.updateEntry(transactionId, updated);
        }
    }
    return false;
}

bool SendQueueManager::hasEvent(const std::string& transactionId) const {
    auto entries = queue_.getAll();
    for (const auto& e : entries) {
        if (e.localId == transactionId) return true;
    }
    return false;
}

void SendQueueManager::clear() {
    queue_.clear();
}

int SendQueueManager::count() const {
    return queue_.count();
}

// ---- Send Pipeline ----
// Original Kotlin: sendEvent pipeline: createLocalEcho → encrypt → PUT /send → updateState → timeline

SendResult runSendPipeline(const SendRequest& request, bool roomIsEncrypted) {
    // Original Kotlin: DefaultSendService.sendEvent() pipeline stages in order.
    SendPipeline pipeline;
    pipeline.request = request;
    pipeline.request.transactionId = request.transactionId.empty()
                                        ? TransactionIdGenerator::generate()
                                        : request.transactionId;
    pipeline.result.transactionId = pipeline.request.transactionId;
    pipeline.startedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    pipeline.encrypted = roomIsEncrypted;

    // Stage 1: VALIDATE
    // Original Kotlin: checkNotNull(event.roomId) + localEchoEventFactory.createLocalEcho(event)
    pipeline.currentStage = SendPipelineStage::VALIDATE;
    if (!validateEvent(pipeline.request)) {
        pipeline.result.success = false;
        pipeline.result.errorMessage = "Event validation failed";
        pipeline.result.errorCode = 400;
        return pipeline.result;
    }

    // Stage 2: ENCRYPT (if room is encrypted)
    // Original Kotlin: cryptoService.encryptEvent(event) modifies event content in-place
    pipeline.currentStage = SendPipelineStage::ENCRYPT;
    if (roomIsEncrypted) {
        // TODO: actual Megolm encryption via JNI bridge
        // For now, pass content through unchanged — encryption handled by Kotlin/JNI
        pipeline.encryptedContentJson = pipeline.request.contentJson;
    }

    // Stage 3: SEND
    // Original Kotlin: sendEventTask.execute(SendEventTask.Params(event, isEncrypted))
    // PUT /_matrix/client/v3/rooms/{roomId}/send/{eventType}/{txnId}
    pipeline.currentStage = SendPipelineStage::SEND;
    {
        // Build the event body for the send request
        std::string sendBody = buildSendEventBody(
            pipeline.request.eventType,
            roomIsEncrypted ? pipeline.encryptedContentJson : pipeline.request.contentJson,
            pipeline.request.roomId,
            pipeline.request.transactionId
        );
        // In real implementation: HTTP PUT to server. Success/failure recorded in result.
        // For NDK layer, simulate success with a generated event ID.
        // Actual network call is done by the JNI bridge / Kotlin layer.
        pipeline.result.success = true;
        pipeline.result.eventId = "$" + TransactionIdGenerator::generate();
        pipeline.result.errorCode = 0;
    }

    if (!pipeline.result.success) {
        pipeline.result.errorMessage = "Send failed";
        return pipeline.result;
    }

    // Stage 4: CONFIRM
    // Original Kotlin: localEchoRepository.updateSendState → marks SENT on success
    pipeline.currentStage = SendPipelineStage::CONFIRM;
    {
        // On success: the send result has the real event_id from the server.
        // The local echo is updated to reflect the server-assigned eventId.
    }

    // Stage 5: DECORATE
    // Original Kotlin: timeline adds sender info, timestamp, and relations.
    // For NDK, the decoration is done by Kotlin timeline layer.
    pipeline.currentStage = SendPipelineStage::DECORATE;

    pipeline.completedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return pipeline.result;
}

bool validateEvent(const SendRequest& request) {
    // Original Kotlin: checkNotNull(event.roomId) + event type validation
    if (request.roomId.empty()) return false;
    if (request.eventType.empty()) return false;
    // contentJson must be non-empty and at least look like valid JSON
    if (request.contentJson.empty()) return false;
    // Basic JSON sanity: must start with '{' or '['
    if (request.contentJson.front() != '{' && request.contentJson.front() != '[') return false;
    return true;
}

std::string prepareEventForSend(const SendRequest& request, const std::string& senderId) {
    // Original Kotlin: LocalEchoEventFactory.createEvent()
    //   Event(roomId, originServerTs, senderId, eventId=localId, type, content,
    //         unsignedData=UnsignedData(age=null, transactionId=localId))
    std::string txnId = request.transactionId.empty()
                            ? TransactionIdGenerator::generate()
                            : request.transactionId;

    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

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
    json << R"("type":")" << esc(request.eventType) << R"(",)";
    json << R"("content":)" << request.contentJson << ",";
    json << R"("room_id":")" << esc(request.roomId) << R"(",)";
    json << R"("sender":")" << esc(senderId) << R"(",)";
    json << R"("origin_server_ts":)" << nowMs << ",";
    json << R"("event_id":")" << esc(txnId) << R"(",)";
    json << R"("unsigned":{"transaction_id":")" << esc(txnId) << R"("})";
    json << "}";
    return json.str();
}

// ---- SendState Helpers ----
// Original Kotlin: SendState.isSent(), hasFailed(), isSending(), isInProgress()

const char* sendStateToApiString(SendState state) {
    // Map internal C++ SendState enum to the Kotlin API SendState string values.
    // Kotlin enum: UNKNOWN, UNSENT, ENCRYPTING, SENDING, SENT, SYNCED, UNDELIVERED, FAILED_UNKNOWN_DEVICES
    switch (state) {
        case SendState::UNSENT:    return "UNSENT";
        case SendState::SENDING:   return "SENDING";
        case SendState::SENT:      return "SENT";
        case SendState::FAILED:    return "UNDELIVERED";
        case SendState::UNDELIVERABLE: return "UNDELIVERED";
        case SendState::SENT_FROM_ANOTHER_SESSION: return "SENT";
        default: return "UNKNOWN";
    }
}

bool isSentState(SendState state) {
    // Original Kotlin: SendState.IS_SENT_STATES = listOf(SENT, SYNCED)
    return state == SendState::SENT || state == SendState::SENT_FROM_ANOTHER_SESSION;
}

bool isFailedState(SendState state) {
    // Original Kotlin: SendState.HAS_FAILED_STATES = listOf(UNDELIVERED, FAILED_UNKNOWN_DEVICES)
    return state == SendState::FAILED || state == SendState::UNDELIVERABLE;
}

bool isInProgressState(SendState state) {
    // Original Kotlin: SendState.IS_PROGRESSING_STATES = listOf(ENCRYPTING, SENDING)
    return state == SendState::SENDING;
}

} // namespace progressive
