#include "progressive/notif_priority.hpp"
#include <sstream>
#include <regex>
#include <algorithm>

namespace progressive {

bool isRoomMention(const std::string& body) {
    auto lower = body;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower.find("@room") != std::string::npos ||
           lower.find("@here") != std::string::npos ||
           lower.find("@channel") != std::string::npos ||
           lower.find("@everyone") != std::string::npos;
}

NotifPriority computeNotifPriority(const NotifContent& content,
    bool isBackground, bool doNotDisturb, bool isFavoriteRoom
) {
    NotifPriority priority;

    // DND overrides everything
    if (doNotDisturb) {
        priority.level = NotifLevel::Silent;
        priority.category = "silent";
        return priority;
    }

    // Calls are always urgent
    if (content.isCallInvite) {
        priority.level = NotifLevel::Urgent;
        priority.shouldVibrate = true;
        priority.shouldWakeScreen = true;
        priority.ledColor = 0xFFFF0000; // red
        priority.soundUri = "ring";
        priority.category = "call";
        return priority;
    }

    // Direct @mention → high
    if (content.isMention) {
        priority.level = NotifLevel::High;
        priority.shouldVibrate = true;
        priority.ledColor = 0xFF00FF00; // green
        priority.category = "msg";
        return priority;
    }

    // @room mention → high
    if (content.isRoomMention) {
        priority.level = NotifLevel::High;
        priority.shouldVibrate = true;
        priority.shouldWakeScreen = true;
        priority.category = "msg";
        return priority;
    }

    // Custom keyword match → normal-high
    if (content.isKeywordMatch) {
        priority.level = isBackground ? NotifLevel::High : NotifLevel::Normal;
        priority.shouldVibrate = isBackground;
        priority.category = "msg";
        return priority;
    }

    // Direct messages → normal
    if (content.isDirectMessage) {
        priority.level = NotifLevel::Normal;
        priority.shouldVibrate = isBackground;
        priority.category = "msg";
        return priority;
    }

    // Favorite rooms → normal even in background
    if (isFavoriteRoom) {
        priority.level = NotifLevel::Normal;
        priority.category = "msg";
        return priority;
    }

    // Regular room messages → low in background, normal in foreground
    priority.level = isBackground ? NotifLevel::Low : NotifLevel::Normal;
    priority.category = "msg";
    return priority;
}

std::string formatNotifTitle(const NotifContent& content) {
    if (content.isDirectMessage) {
        return content.senderName;
    }
    if (!content.roomName.empty()) {
        return content.roomName;
    }
    return content.senderName;
}

std::string formatNotifBody(const NotifContent& content, bool showSender) {
    std::ostringstream out;
    if (showSender && content.isDirectMessage) {
        // For DMs, just show body
        out << content.body;
    } else if (showSender && !content.isDirectMessage) {
        out << content.senderName << ": " << content.body;
    } else {
        out << content.body;
    }
    return out.str();
}

std::string truncateForNotification(const std::string& text, int maxLen) {
    if (static_cast<int>(text.size()) <= maxLen) return text;
    // Try to truncate at a word boundary
    auto truncated = text.substr(0, maxLen);
    auto lastSpace = truncated.rfind(' ');
    if (lastSpace != std::string::npos && lastSpace > maxLen * 2 / 3) {
        truncated = truncated.substr(0, lastSpace);
    }
    return truncated + "...";
}

} // namespace progressive
