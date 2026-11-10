#include "pcp/message_scheduler.hpp"
#include <sstream>
#include <chrono>
#include <cstdlib>

namespace pcp {

MessageScheduler::MessageScheduler() {}

std::string MessageScheduler::generateId() const {
    return "sched_" + std::to_string(nowMs()) + "_" + std::to_string(rand());
}

int64_t MessageScheduler::nowMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string MessageScheduler::schedule(const std::string& roomId, const std::string& body,
                                        const std::string& formattedBody, int64_t triggerAtMs) {
    ScheduledMessage msg;
    msg.id = generateId();
    msg.roomId = roomId;
    msg.body = body;
    msg.formattedBody = formattedBody;
    msg.scheduledAtMs = triggerAtMs;
    msg.createdAtMs = nowMs();
    messages_.push_back(msg);
    return msg.id;
}

void MessageScheduler::cancel(const std::string& id) {
    for (auto& m : messages_) {
        if (m.id == id && !m.sent) { m.cancelled = true; break; }
    }
}

std::vector<ScheduledMessage> MessageScheduler::getPending() const {
    std::vector<ScheduledMessage> result;
    for (const auto& m : messages_) {
        if (!m.sent && !m.cancelled) result.push_back(m);
    }
    return result;
}

std::vector<ScheduledMessage> MessageScheduler::getDue() {
    std::vector<ScheduledMessage> result;
    int64_t now = nowMs();
    for (auto& m : messages_) {
        if (!m.sent && !m.cancelled && m.scheduledAtMs <= now) {
            m.sent = true;
            result.push_back(m);
        }
    }
    return result;
}

std::vector<ScheduledMessage> MessageScheduler::getForRoom(const std::string& roomId) const {
    std::vector<ScheduledMessage> result;
    for (const auto& m : messages_) {
        if (m.roomId == roomId && !m.sent && !m.cancelled) result.push_back(m);
    }
    return result;
}

std::string MessageScheduler::toJson() const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < messages_.size(); i++) {
        if (i > 0) os << ",";
        const auto& m = messages_[i];
        os << "{";
        os << "\"id\":\"" << m.id << "\"";
        os << ",\"roomId\":\"" << m.roomId << "\"";
        os << ",\"body\":\"" << m.body << "\"";
        os << ",\"formattedBody\":\"" << m.formattedBody << "\"";
        os << ",\"scheduledAtMs\":" << m.scheduledAtMs;
        os << ",\"createdAtMs\":" << m.createdAtMs;
        os << ",\"sent\":" << (m.sent ? "true" : "false");
        os << ",\"cancelled\":" << (m.cancelled ? "true" : "false");
        os << "}";
    }
    os << "]";
    return os.str();
}

void MessageScheduler::fromJson(const std::string& json) {
    messages_.clear();
    if (json.empty() || json == "[]") return;
    auto extractStr = [&](const std::string& haystack, const std::string& key, size_t startPos) -> std::string {
        auto p = haystack.find("\"" + key + "\":\"", startPos);
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = haystack.find('"', p);
        if (e == std::string::npos) return "";
        return haystack.substr(p, e - p);
    };
    auto extractInt = [&](const std::string& haystack, const std::string& key, size_t startPos) -> int64_t {
        auto p = haystack.find("\"" + key + "\":", startPos);
        if (p == std::string::npos) return 0;
        p += key.size() + 2;
        while (p < haystack.size() && haystack[p] == ' ') p++;
        try { return std::stoll(haystack.substr(p)); } catch (...) { return 0; }
    };
    auto extractBool = [&](const std::string& haystack, const std::string& key, size_t startPos) -> bool {
        auto p = haystack.find("\"" + key + "\":", startPos);
        if (p == std::string::npos) return false;
        p += key.size() + 2;
        while (p < haystack.size() && haystack[p] == ' ') p++;
        return haystack.substr(p, 4) == "true";
    };

    size_t pos = 0;
    while (true) {
        pos = json.find("\"id\":\"", pos);
        if (pos == std::string::npos) break;
        ScheduledMessage m;
        m.id = extractStr(json, "id", pos);
        m.roomId = extractStr(json, "roomId", pos);
        m.body = extractStr(json, "body", pos);
        m.formattedBody = extractStr(json, "formattedBody", pos);
        m.scheduledAtMs = extractInt(json, "scheduledAtMs", pos);
        m.createdAtMs = extractInt(json, "createdAtMs", pos);
        m.sent = extractBool(json, "sent", pos);
        m.cancelled = extractBool(json, "cancelled", pos);
        messages_.push_back(m);
        pos = json.find('}', pos) + 1;
    }
}

void MessageScheduler::cleanup(int64_t olderThanMs) {
    int64_t threshold = olderThanMs > 0 ? olderThanMs : nowMs() - 86400000LL * 7; // 7 days
    messages_.erase(std::remove_if(messages_.begin(), messages_.end(),
        [threshold](const ScheduledMessage& m) {
            return (m.sent || m.cancelled) && m.createdAtMs < threshold;
        }), messages_.end());
}

} // namespace pcp
