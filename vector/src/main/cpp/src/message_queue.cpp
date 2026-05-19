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

} // namespace progressive
