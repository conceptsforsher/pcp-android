#ifndef PROGRESSIVE_CHAT_PREVIEW_HPP
#define PROGRESSIVE_CHAT_PREVIEW_HPP

#include <string>
#include <vector>

namespace progressive {

struct PreviewEvent {
    std::string senderName;
    std::string body;          // message text
    std::string timestamp;     // short time like "12:34"
    bool isPending = false;    // sending/failed
    bool hasAttachment = false;
    std::string attachmentType; // "image", "video", "file", "audio"
    std::string senderId;      // for dedup
};

struct ChatPreviewState {
    bool hasUnread = false;
    bool hasPing = false;
    int unreadCount = 0;
    std::string lastMessage;    // single-line preview (current behavior)
    std::string lastSender;
    std::string lastTimestamp;
    bool isPublic = false;

    // Expanded 2-3x block content
    std::vector<PreviewEvent> compactMessages; // up to 5 messages
    bool showDraft = false;
    std::string draftText;

    // Format the compact preview as JSON string for Kotlin
    std::string toJson() const;
};

// Build a chat preview state from a raw list of recent events.
// Returns a preview that can be rendered as single block (current)
// or expanded 2-3x block (compactMessages filled).
ChatPreviewState buildChatPreview(
    const std::vector<PreviewEvent>& recentEvents,
    int unreadCount,
    bool hasPing,
    bool isPublic
);

// Truncate message body to fit in compact preview.
std::string truncateMessage(const std::string& body, int maxLen = 60);

// Format a timestamp to short time: "12:34"
std::string formatShortTime(int64_t epochMs);

} // namespace progressive

#endif // PROGRESSIVE_CHAT_PREVIEW_HPP
