#pragma once
#include <string>
#include <cstdint>

namespace pcp {

struct TextStats {
    int charCount = 0;
    int wordCount = 0;
    int sentenceCount = 0;
    int lineCount = 0;
    int emojiCount = 0;
    int urlCount = 0;
    int estimatedReadTimeSec = 0;
};

TextStats computeTextStats(const std::string& text);
std::string textStatsToJson(const TextStats& stats);
int estimateReadTimeMs(const std::string& text);

} // namespace pcp
