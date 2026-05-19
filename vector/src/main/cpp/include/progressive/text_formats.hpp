#ifndef PROGRESSIVE_TEXT_FORMATS_HPP
#define PROGRESSIVE_TEXT_FORMATS_HPP

#include <string>

namespace progressive {

// ---- Text Formatting Commands ----

// Format text as a spoiler: wraps in <span data-mx-spoiler>
std::string formatSpoiler(const std::string& text);

// Format text as a /me emote: italic with sender prefix
std::string formatEmote(const std::string& senderName, const std::string& text);

// Format text with /shrug: text + ¯\_(ツ)_/¯
std::string formatShrug(const std::string& text);

// Format text with /lenny: text + ( ͡° ͜ʖ ͡°)
std::string formatLenny(const std::string& text);

// Format text with /tableflip: text + (╯°□°）╯︵ ┻━┻
std::string formatTableFlip(const std::string& text);

// Format plain text (strip formatting markers)
std::string formatPlain(const std::string& text);

// Escape HTML entities in text.
std::string escapeHtml(const std::string& text);

// Truncate text to maxLen characters, adding "..." if needed.
// Respects UTF-8 multi-byte boundary.
std::string truncateText(const std::string& text, int maxLen);

// Count UTF-8 characters in a string.
int utf8CharCount(const std::string& text);

// Check if text contains only emoji (for emoji-only messages).
bool isEmojiOnly(const std::string& text);

// Format duration as HH:MM:SS or MM:SS.
std::string formatDuration(int64_t totalSeconds);

// Format duration with units: "1h 30min 15sec"
std::string formatDurationWithUnits(int64_t totalSeconds, bool appendSeconds = true);

// Format unread jump label: "5 new messages (2 mentions)"
inline std::string formatUnreadJumpLabel(int unreadCount, int unreadMentions) {
    std::string result = std::to_string(unreadCount);
    if (unreadCount == 1) result += " new message";
    else result += " new messages";
    if (unreadMentions > 0) result += " (" + std::to_string(unreadMentions) + " mentions)";
    return result;
}

// Check if read marker JSON indicates unread messages
inline bool shouldShowJumpToUnread(const std::string& readMarkerJson) {
    return readMarkerJson.find("\"hasUnread\": true") != std::string::npos ||
           readMarkerJson.find("\"hasUnread\":true") != std::string::npos;
}

} // namespace progressive

#endif // PROGRESSIVE_TEXT_FORMATS_HPP
