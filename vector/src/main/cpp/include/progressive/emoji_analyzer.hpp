#ifndef PROGRESSIVE_EMOJI_ANALYZER_HPP
#define PROGRESSIVE_EMOJI_ANALYZER_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct EmojiUsage {
    std::string emoji;
    int count = 0;
    std::string category;     // "smiley", "gesture", "heart", etc.
    double frequency = 0.0;   // percentage of all emoji
};

struct EmojiStats {
    int totalEmoji = 0;
    int uniqueEmoji = 0;
    std::vector<EmojiUsage> topEmojis;     // top 10 by usage
    std::string favoriteEmoji;
    std::vector<std::string> recentEmojis; // last 10 used
};

// Analyze emoji usage from a list.
EmojiStats analyzeEmojiUsage(const std::vector<std::string>& emojis);

// Extract all emojis from text.
std::vector<std::string> extractEmojis(const std::string& text);

// Check if text consists entirely of emojis.
bool isEmojiOnlyMessage(const std::string& text, int maxNonEmojiChars = 3);

// Get emoji category.
std::string getEmojiCategory(const std::string& emoji);

// Check if a character is in the emoji Unicode range.
bool isEmojiChar(uint32_t codepoint);

// Find most similar emoji (for search/autocomplete).
std::string findSimilarEmoji(const std::string& query,
    const std::vector<std::string>& emojiList, int maxResults = 5);

} // namespace progressive

#endif // PROGRESSIVE_EMOJI_ANALYZER_HPP
