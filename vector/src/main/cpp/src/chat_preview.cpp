#include "progressive/chat_preview.hpp"
#include <sstream>
#include <ctime>

namespace progressive {

std::string truncateMessage(const std::string& body, int maxLen) {
    if (static_cast<int>(body.size()) <= maxLen) return body;
    return body.substr(0, maxLen - 3) + "...";
}

std::string formatShortTime(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    localtime_r(&ts, &result);
    char buf[8];
    strftime(buf, sizeof(buf), "%H:%M", &result);
    return std::string(buf);
}

ChatPreviewState buildChatPreview(
    const std::vector<PreviewEvent>& recentEvents,
    int unreadCount,
    bool hasPing,
    bool isPublic
) {
    ChatPreviewState state;
    state.hasUnread = unreadCount > 0;
    state.hasPing = hasPing;
    state.unreadCount = unreadCount;
    state.isPublic = isPublic;

    if (recentEvents.empty()) {
        state.lastMessage = "";
        return state;
    }

    // Last message for single-line preview
    const auto& last = recentEvents.back();
    state.lastSender = last.senderName;
    state.lastTimestamp = last.timestamp;
    if (last.hasAttachment) {
        state.lastMessage = "[" + last.attachmentType + "]";
        if (!last.body.empty()) state.lastMessage += " " + last.body;
    } else {
        state.lastMessage = last.body;
    }

    // Build compact messages for expanded block (last 5 messages)
    int count = static_cast<int>(recentEvents.size());
    int start = count > 5 ? count - 5 : 0;
    std::string prevSender;

    for (int i = start; i < count; ++i) {
        PreviewEvent ev = recentEvents[i];
        // Truncate long messages for compact display
        int maxCompactLen = (i == count - 1) ? 60 : 50;
        ev.body = truncateMessage(ev.body, maxCompactLen);
        state.compactMessages.push_back(ev);
    }

    return state;
}

std::string ChatPreviewState::toJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("hasUnread": )" << (hasUnread ? "true" : "false") << ",";
    json << R"("hasPing": )" << (hasPing ? "true" : "false") << ",";
    json << R"("unreadCount": )" << unreadCount << ",";
    json << R"("lastMessage": ")" << esc(lastMessage) << R"(",)";
    json << R"("lastSender": ")" << esc(lastSender) << R"(",)";
    json << R"("lastTimestamp": ")" << esc(lastTimestamp) << R"(",)";
    json << R"("isPublic": )" << (isPublic ? "true" : "false") << ",";
    json << R"("compactMessages": [)";
    for (size_t i = 0; i < compactMessages.size(); ++i) {
        if (i > 0) json << ",";
        const auto& m = compactMessages[i];
        json << R"({"sender": ")" << esc(m.senderName) << R"(",)";
        json << R"("body": ")" << esc(m.body) << R"(",)";
        json << R"("time": ")" << esc(m.timestamp) << R"(")";
        if (m.hasAttachment) json << R"(,"attachment": ")" << esc(m.attachmentType) << R"(")";
        json << "}";
    }
    json << "]";
    if (showDraft) {
        json << R"(,"draft": ")" << esc(draftText) << R"(")";
    }
    json << "}";
    return json.str();
}

} // namespace progressive
