#include "progressive/message_queue.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <unordered_set>

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

} // namespace progressive
