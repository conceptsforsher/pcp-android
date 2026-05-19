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

// Original Kotlin: getPreviewIcon — map attachment type to preview icon
PreviewIconType getPreviewIcon(const std::string& attachmentType) {
    if (attachmentType == "image") return PreviewIconType::IMAGE;
    if (attachmentType == "video") return PreviewIconType::VIDEO;
    if (attachmentType == "file") return PreviewIconType::FILE;
    if (attachmentType == "audio") return PreviewIconType::AUDIO;
    if (attachmentType == "sticker") return PreviewIconType::STICKER;
    if (attachmentType == "location") return PreviewIconType::LOCATION;
    if (attachmentType == "poll") return PreviewIconType::POLL;
    if (attachmentType == "call") return PreviewIconType::CALL;
    if (attachmentType == "encrypted") return PreviewIconType::ENCRYPTED;
    if (attachmentType == "member") return PreviewIconType::MEMBER;
    return PreviewIconType::MESSAGE;
}

// Original Kotlin: getPreviewIconForRoom — best icon for room state
PreviewIconType getPreviewIconForRoom(bool isEncrypted, bool isDirect, bool hasUnread, bool isFavourite) {
    if (isEncrypted) return PreviewIconType::ENCRYPTED;
    if (isFavourite) return PreviewIconType::MESSAGE;
    if (isDirect) return PreviewIconType::MESSAGE;
    if (hasUnread) return PreviewIconType::MESSAGE;
    return PreviewIconType::NONE;
}

// Original Kotlin: formatChatPreviewWithConfig — preview with configurable options
std::string formatChatPreviewWithConfig(const ChatPreviewState& state, const ChatPreviewConfig& config) {
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
    json << R"("lastMessage": ")" << esc(state.lastMessage);
    if (config.showEncryptionIcon && state.lastMessage.empty()) {
        json << R"(",)";
        json << R"("encryptionIcon": true)";
    }
    if (config.showFailedIndicator) {
        // Check first compact message for pending failed
        bool hasFailed = false;
        for (const auto& m : state.compactMessages) {
            if (m.isPending) { hasFailed = true; break; }
        }
        if (hasFailed) json << R"(,"hasFailed": true)";
    }
    json << "}";
    return json.str();
}

// Original Kotlin: formatTypingPreview — format typing indicator
std::string formatTypingPreview(const std::vector<std::string>& typingUserNames) {
    if (typingUserNames.empty()) return "";
    if (typingUserNames.size() == 1) {
        return typingUserNames[0] + " is typing...";
    }
    if (typingUserNames.size() == 2) {
        return typingUserNames[0] + " and " + typingUserNames[1] + " are typing...";
    }
    return std::to_string(typingUserNames.size()) + " people are typing...";
}

// Original Kotlin: formatDraftPreview — draft indicator for preview
std::string formatDraftPreview(const std::string& draftText, int maxLen) {
    if (draftText.empty()) return "";
    std::string prefix = "[Draft] ";
    std::string body = draftText;
    // Remove newlines
    for (auto& c : body) { if (c == '\n') c = ' '; }
    if (static_cast<int>(body.size()) > maxLen) {
        body = body.substr(0, maxLen - 3) + "...";
    }
    return prefix + body;
}

// Original Kotlin: formatFailedPreview — failed send indicator
std::string formatFailedPreview(const std::string& originalBody, int maxLen) {
    if (originalBody.empty()) return "[Failed to send]";
    std::string body = originalBody;
    for (auto& c : body) { if (c == '\n') c = ' '; }
    if (static_cast<int>(body.size()) > maxLen) {
        body = body.substr(0, maxLen - 3) + "...";
    }
    return "[Failed] " + body;
}

// Original Kotlin: iconEmoji — Unicode symbol for each PreviewIconType
static const char* iconEmoji(PreviewIconType icon) {
    switch (icon) {
        case PreviewIconType::MESSAGE:   return "\xF0\x9F\x92\xAC"; // 💬
        case PreviewIconType::IMAGE:     return "\xF0\x9F\x96\xBC"; // 🖼
        case PreviewIconType::VIDEO:     return "\xF0\x9F\x8E\xAC"; // 🎬
        case PreviewIconType::FILE:      return "\xF0\x9F\x93\x84"; // 📄
        case PreviewIconType::AUDIO:     return "\xF0\x9F\x8E\xB5"; // 🎵
        case PreviewIconType::STICKER:   return "\xF0\x9F\x98\x8D"; // 😍
        case PreviewIconType::LOCATION:  return "\xF0\x9F\x93\x8D"; // 📍
        case PreviewIconType::POLL:      return "\xF0\x9F\x93\x8A"; // 📊
        case PreviewIconType::CALL:      return "\xF0\x9F\x93\x9E"; // 📞
        case PreviewIconType::ENCRYPTED: return "\xF0\x9F\x94\x92"; // 🔒
        case PreviewIconType::MEMBER:    return "\xF0\x9F\x91\xA4"; // 👤
        case PreviewIconType::TYPING:    return "\xE2\x9C\x8F";     // ✏
        case PreviewIconType::DRAFT:     return "\xF0\x9F\x93\x9D"; // 📝
        case PreviewIconType::FAILED:    return "\xE2\x9A\xA0";     // ⚠
        case PreviewIconType::NONE:
        default:                         return "";
    }
}

// Original Kotlin: buildPreviewLine — construct ChatPreviewLine from event data
ChatPreviewLine buildPreviewLine(const PreviewEvent& event, const ChatPreviewConfig& config) {
    ChatPreviewLine line;
    if (event.hasAttachment) {
        line.icon = getPreviewIcon(event.attachmentType);
    } else if (event.isPending) {
        line.icon = PreviewIconType::FAILED;
        line.color = "#FF5252";
    } else {
        line.icon = PreviewIconType::MESSAGE;
    }
    line.text = event.senderName.empty() ? event.body : (event.senderName + ": " + event.body);
    line.isBold = !event.senderName.empty();
    if (event.isPending) line.color = "#FF5252";
    return line;
}

// Original Kotlin: getPreviewIconName — string name for PreviewIconType
const char* getPreviewIconName(PreviewIconType icon) {
    switch (icon) {
        case PreviewIconType::MESSAGE:   return "message";
        case PreviewIconType::IMAGE:     return "image";
        case PreviewIconType::VIDEO:     return "video";
        case PreviewIconType::FILE:      return "file";
        case PreviewIconType::AUDIO:     return "audio";
        case PreviewIconType::STICKER:   return "sticker";
        case PreviewIconType::LOCATION:  return "location";
        case PreviewIconType::POLL:      return "poll";
        case PreviewIconType::CALL:      return "call";
        case PreviewIconType::ENCRYPTED: return "encrypted";
        case PreviewIconType::MEMBER:    return "member";
        case PreviewIconType::TYPING:    return "typing";
        case PreviewIconType::DRAFT:     return "draft";
        case PreviewIconType::FAILED:    return "failed";
        case PreviewIconType::NONE:
        default:                         return "none";
    }
}

// Original Kotlin: formatPreviewLinesJson — serialize preview lines to JSON
std::string formatPreviewLinesJson(const std::vector<ChatPreviewLine>& lines) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < lines.size(); i++) {
        if (i > 0) json << ",";
        json << R"({"icon": ")" << esc(getPreviewIconName(lines[i].icon))
             << R"(", "text": ")" << esc(lines[i].text)
             << R"(", "isBold": )" << (lines[i].isBold ? "true" : "false");
        if (!lines[i].color.empty())
            json << R"(, "color": ")" << esc(lines[i].color) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

// Original Kotlin: deduplicateSenders — merge consecutive messages from same sender
std::vector<PreviewEvent> deduplicateSenders(const std::vector<PreviewEvent>& events) {
    std::vector<PreviewEvent> result;
    std::string lastSender;
    for (const auto& e : events) {
        if (e.senderId != lastSender || result.empty()) {
            result.push_back(e);
            lastSender = e.senderId;
        } else {
            // Append body to previous event
            auto& prev = result.back();
            if (!e.body.empty()) {
                if (!prev.body.empty()) prev.body += "\n";
                prev.body += e.body;
            }
            if (e.hasAttachment) {
                prev.hasAttachment = true;
                prev.attachmentType = e.attachmentType;
            }
        }
    }
    return result;
}

} // namespace progressive
