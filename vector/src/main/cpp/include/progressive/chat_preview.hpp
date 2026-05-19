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

// Original Kotlin: PreviewIconType enum for chat preview icons
enum class PreviewIconType {
    MESSAGE,
    IMAGE,
    VIDEO,
    FILE,
    AUDIO,
    STICKER,
    LOCATION,
    POLL,
    CALL,
    ENCRYPTED,
    MEMBER,
    TYPING,
    DRAFT,
    FAILED,
    NONE
};

// Original Kotlin: getPreviewIcon — map attachment type to icon
PreviewIconType getPreviewIcon(const std::string& attachmentType);

// Original Kotlin: ChatPreviewConfig for customizing preview display
struct ChatPreviewConfig {
    int maxBodyLength = 60;
    bool showEncryptionIcon = true;
    bool showTypingIndicator = true;
    bool showDraft = true;
    bool showFailedIndicator = true;
};

// Original Kotlin: ChatPreviewLine — single line in a preview block
struct ChatPreviewLine {
    PreviewIconType icon = PreviewIconType::NONE;
    std::string text;
    bool isBold = false;
    std::string color; // hex color or empty for default
};

// Original Kotlin: formatChatPreviewWithConfig
std::string formatChatPreviewWithConfig(const ChatPreviewState& state, const ChatPreviewConfig& config);

// Original Kotlin: getPreviewIconForRoom — determine best icon for room
PreviewIconType getPreviewIconForRoom(bool isEncrypted, bool isDirect, bool hasUnread, bool isFavourite);

// Original Kotlin: formatTypingPreview — "User is typing..."
std::string formatTypingPreview(const std::vector<std::string>& typingUserNames);

// Original Kotlin: formatDraftPreview — show draft message preview
std::string formatDraftPreview(const std::string& draftText, int maxLen = 40);

// Original Kotlin: formatFailedPreview — show failed send indicator
std::string formatFailedPreview(const std::string& originalBody, int maxLen = 40);

// Original Kotlin: buildPreviewLine — construct a ChatPreviewLine from event data
ChatPreviewLine buildPreviewLine(const PreviewEvent& event, const ChatPreviewConfig& config);

// Original Kotlin: getPreviewIconName — string name for PreviewIconType
const char* getPreviewIconName(PreviewIconType icon);

// Original Kotlin: formatPreviewLinesJson — serialize preview lines to JSON
std::string formatPreviewLinesJson(const std::vector<ChatPreviewLine>& lines);

// Original Kotlin: deduplicateSenders — merge consecutive messages from same sender
std::vector<PreviewEvent> deduplicateSenders(const std::vector<PreviewEvent>& events);

} // namespace progressive

#endif // PROGRESSIVE_CHAT_PREVIEW_HPP
