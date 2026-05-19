#include "progressive/user_messages.hpp"
#include <algorithm>
#include <sstream>

namespace progressive {

std::vector<const UserMessage*> getLatestMessages(
    const std::vector<UserMessage>& allMessages,
    int limit
) {
    // Create pointers sorted by timestamp descending
    std::vector<const UserMessage*> sorted;
    sorted.reserve(allMessages.size());
    for (const auto& msg : allMessages) {
        sorted.push_back(&msg);
    }
    std::sort(sorted.begin(), sorted.end(), [](const UserMessage* a, const UserMessage* b) {
        return a->originServerTs > b->originServerTs;
    });
    if (static_cast<int>(sorted.size()) > limit) {
        sorted.resize(limit);
    }
    return sorted;
}

std::string userMessagesToJson(const std::vector<const UserMessage*>& messages) {
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
    json << "[";
    for (size_t i = 0; i < messages.size(); ++i) {
        if (i > 0) json << ",";
        const auto& m = *messages[i];
        json << "{";
        json << R"("eventId": ")" << esc(m.eventId) << R"(",)";
        json << R"("roomId": ")" << esc(m.roomId) << R"(",)";
        json << R"("roomName": ")" << esc(m.roomName) << R"(",)";
        json << R"("body": ")" << esc(m.body) << R"(",)";
        json << R"("timestamp": ")" << esc(m.timestamp) << R"(",)";
        json << R"("msgType": ")" << esc(m.msgType) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

std::string formatUserMessagePreview(const UserMessage& msg, int maxLen) {
    std::ostringstream out;
    out << msg.roomName << ": ";
    if (msg.msgType == "m.image") out << "[image] ";
    else if (msg.msgType == "m.video") out << "[video] ";
    else if (msg.msgType == "m.file") out << "[file] ";
    else if (msg.msgType == "m.audio") out << "[audio] ";

    std::string body = msg.body;
    if (static_cast<int>(body.size()) > maxLen) {
        body = body.substr(0, maxLen) + "...";
    }
    out << body;
    return out.str();
}

} // namespace progressive
