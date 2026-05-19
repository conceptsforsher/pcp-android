#include "progressive/emoji_analyzer.hpp"
#include "progressive/content_guard.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

bool isEmojiChar(uint32_t cp) {
    return (cp >= 0x1F600 && cp <= 0x1F64F) || // emoticons
           (cp >= 0x1F300 && cp <= 0x1F5FF) || // misc symbols
           (cp >= 0x1F680 && cp <= 0x1F6FF) || // transport
           (cp >= 0x2600 && cp <= 0x27BF) ||   // misc
           (cp >= 0x1F900 && cp <= 0x1F9FF) || // supplemental
           (cp >= 0x1FA00 && cp <= 0x1FA6F) || // chess
           (cp >= 0x1FA70 && cp <= 0x1FAFF) || // extended-A
           (cp >= 0x200D && cp <= 0x200D);      // ZWJ
}

static int utf8CharLen(unsigned char c) {
    if (c < 0x80) return 1;
    if (c < 0xE0) return 2;
    if (c < 0xF0) return 3;
    return 4;
}

static uint32_t utf8ToCodepoint(const std::string& s, size_t& i) {
    unsigned char c = s[i];
    uint32_t cp = 0;
    if (c < 0x80) { cp = c; }
    else if (c < 0xE0) { cp = ((c & 0x1F) << 6) | (s[i+1] & 0x3F); i += 1; }
    else if (c < 0xF0) { cp = ((c & 0x0F) << 12) | ((s[i+1] & 0x3F) << 6) | (s[i+2] & 0x3F); i += 2; }
    else { cp = ((c & 0x07) << 18) | ((s[i+1] & 0x3F) << 12) | ((s[i+2] & 0x3F) << 6) | (s[i+3] & 0x3F); i += 3; }
    return cp;
}

std::vector<std::string> extractEmojis(const std::string& text) {
    std::vector<std::string> result;
    for (size_t i = 0; i < text.size();) {
        size_t start = i;
        int len = utf8CharLen(text[i]);
        if (i + len > text.size()) break;
        uint32_t cp = utf8ToCodepoint(text, i);
        i += len;
        if (isEmojiChar(cp)) {
            result.push_back(text.substr(start, i - start));
        }
    }
    return result;
}

bool isEmojiOnlyMessage(const std::string& text, int maxNonEmojiChars) {
    int nonEmoji = 0;
    for (size_t i = 0; i < text.size();) {
        int len = utf8CharLen(text[i]);
        if (i + len > text.size()) break;
        uint32_t cp = utf8ToCodepoint(text, i);
        i += len;

        if (!isEmojiChar(cp) && cp != ' ' && cp != '\n') nonEmoji++;
        if (nonEmoji > maxNonEmojiChars) return false;
    }
    return countEmojis(text) > 0;
}

EmojiStats analyzeEmojiUsage(const std::vector<std::string>& emojis) {
    EmojiStats stats;
    stats.totalEmoji = static_cast<int>(emojis.size());

    std::unordered_map<std::string, int> counter;
    for (const auto& e : emojis) counter[e]++;

    stats.uniqueEmoji = static_cast<int>(counter.size());

    // Top emojis
    std::vector<std::pair<std::string, int>> sorted(counter.begin(), counter.end());
    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int topN = std::min(10, static_cast<int>(sorted.size()));
    for (int i = 0; i < topN; ++i) {
        EmojiUsage eu;
        eu.emoji = sorted[i].first;
        eu.count = sorted[i].second;
        eu.frequency = stats.totalEmoji > 0 ? (eu.count * 100.0) / stats.totalEmoji : 0.0;
        eu.category = getEmojiCategory(eu.emoji);
        stats.topEmojis.push_back(eu);
    }

    if (!sorted.empty()) stats.favoriteEmoji = sorted[0].first;

    // Recent emojis (last 10)
    int recentStart = std::max(0, stats.totalEmoji - 10);
    for (int i = recentStart; i < stats.totalEmoji; ++i) {
        if (i < static_cast<int>(emojis.size())) {
            stats.recentEmojis.push_back(emojis[i]);
        }
    }

    return stats;
}

std::string getEmojiCategory(const std::string& emoji) {
    if (emoji.empty()) return "unknown";
    uint32_t cp = 0;
    size_t i = 0;
    cp = utf8ToCodepoint(emoji, i);

    if (cp >= 0x1F600 && cp <= 0x1F64F) return "smiley";
    if (cp >= 0x1F300 && cp <= 0x1F5FF) return "misc";
    if (cp >= 0x1F680 && cp <= 0x1F6FF) return "transport";
    if (cp >= 0x2764 && cp <= 0x2764) return "heart";
    if (cp >= 0x270A && cp <= 0x270C) return "gesture";
    if ((cp >= 0x1F440 && cp <= 0x1F4A9) || (cp >= 0x1F44B && cp <= 0x1F4F7)) return "people";
    if (cp >= 0x1F400 && cp <= 0x1F43F) return "animal";
    if (cp >= 0x1F32D && cp <= 0x1F37F) return "food";
    if (cp >= 0x1F3B5 && cp <= 0x1F3BC) return "music";
    if (cp >= 0x2694 && cp <= 0x26F9) return "symbol";
    return "other";
}

std::string findSimilarEmoji(const std::string& query,
    const std::vector<std::string>& emojiList, int maxResults) {
    if (query.empty() || emojiList.empty()) return "";

    auto lower = query;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Simple substring match on emoji character
    for (const auto& emoji : emojiList) {
        if (emoji.find(query) != std::string::npos) return emoji;
    }
    return emojiList[0];
}

} // namespace progressive
