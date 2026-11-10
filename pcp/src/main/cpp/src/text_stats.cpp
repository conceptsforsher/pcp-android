#include "pcp/text_stats.hpp"
#include <sstream>

namespace pcp {

TextStats computeTextStats(const std::string& text) {
    TextStats s;
    s.charCount = (int)text.size();
    bool inWord = false;
    for (char c : text) {
        if (c == '\n') s.lineCount++;
        if (c == '.' || c == '!' || c == '?') s.sentenceCount++;
        if (c == ' ' || c == '\n' || c == '\t') { if (inWord) s.wordCount++; inWord = false; }
        else inWord = true;
    }
    if (inWord) s.wordCount++;
    s.estimatedReadTimeSec = s.wordCount / 3; // 3 words/sec
    if (s.estimatedReadTimeSec < 1) s.estimatedReadTimeSec = 1;
    return s;
}

std::string textStatsToJson(const TextStats& stats) {
    std::ostringstream os;
    os << "{" << R"("chars":)" << stats.charCount << R"(,"words":)" << stats.wordCount
       << R"(,"sentences":)" << stats.sentenceCount << R"(,"readTimeSec":)" << stats.estimatedReadTimeSec << "}";
    return os.str();
}

int estimateReadTimeMs(const std::string& text) {
    return computeTextStats(text).estimatedReadTimeSec * 1000;
}

} // namespace pcp
