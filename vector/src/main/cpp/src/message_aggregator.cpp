#include "progressive/message_aggregator.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

void MessageAggregator::addMessage(const AggregatedMessage& msg) {
    // Check if already exists
    for (auto& m : messages_) {
        if (m.eventId == msg.eventId) {
            m = msg;
            return;
        }
    }
    messages_.push_back(msg);
}

std::vector<AggregatedMessage> MessageAggregator::getAllMessages() const {
    auto result = messages_;
    std::sort(result.begin(), result.end(), [](const AggregatedMessage& a, const AggregatedMessage& b) {
        return a.originServerTs > b.originServerTs;
    });
    return result;
}

void MessageAggregator::removeRoom(const std::string& roomId) {
    messages_.erase(std::remove_if(messages_.begin(), messages_.end(),
        [&](const AggregatedMessage& m) { return m.roomId == roomId; }
    ), messages_.end());
}

void MessageAggregator::removeAccount(const std::string& accountId) {
    messages_.erase(std::remove_if(messages_.begin(), messages_.end(),
        [&](const AggregatedMessage& m) { return m.accountId == accountId; }
    ), messages_.end());
}

void MessageAggregator::clear() {
    messages_.clear();
}

std::string MessageAggregator::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    auto msgs = getAllMessages();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < msgs.size(); ++i) {
        if (i > 0) json << ",";
        const auto& m = msgs[i];
        json << "{";
        json << R"("eventId": ")" << esc(m.eventId) << R"(",)";
        json << R"("roomId": ")" << esc(m.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(m.roomName) << R"(",)";
        json << R"("accountId": ")" << esc(m.accountId) << R"(",)";
        json << R"("accountIndex": ")" << esc(m.accountIndex) << R"(",)";
        json << R"("senderName": ")" << esc(m.senderName) << R"(",)";
        json << R"("body": ")" << esc(m.body) << R"(",)";
        json << R"("msgType": ")" << esc(m.msgType) << R"(",)";
        json << R"("timestamp": ")" << esc(m.timestamp) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

} // namespace progressive
