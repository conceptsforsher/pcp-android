#include "pcp/string_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace pcp {

std::string sanitizeRoomName(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c >= 32 && c <= 126) {
            result += c;
        }
    }
    // Collapse multiple spaces
    std::regex multiSpace("  +");
    result = std::regex_replace(result, multiSpace, " ");
    return trim(result);
}

std::string replaceSpaceChars(const std::string& input) {
    std::string result = input;
    // Replace various Unicode space characters with regular space
    std::vector<std::string> spaces = {
        "\u00A0", "\u2000", "\u2001", "\u2002", "\u2003",
        "\u2004", "\u2005", "\u2006", "\u2007", "\u2008",
        "\u2009", "\u200A", "\u202F", "\u205F", "\u3000"
    };
    for (const auto& sp : spaces) {
        size_t pos = 0;
        while ((pos = result.find(sp, pos)) != std::string::npos) {
            result.replace(pos, sp.size(), " ");
            pos++;
        }
    }
    return result;
}

// ==== Count Formatter (from TextUtils.kt:30-45) ====
// Original Kotlin:
//   fun formatCountToShortDecimal(value: Int): String {
//       if (value < 0) return "-" + formatCountToShortDecimal(-value)
//       if (value < 1000) return value.toString()
//       val e = suffixes.floorEntry(value)
//       val divideBy = e?.key
//       val suffix = e?.value
//       val truncated = value / (divideBy!! / 10)
//       val hasDecimal = truncated < 100 && truncated / 10.0 != (truncated / 10).toDouble()
//       return if (hasDecimal) "${truncated / 10.0}$suffix" else "${truncated / 10}$suffix"
//   }

std::string trim(const std::string& input) {
    size_t start = 0;
    while (start < input.size() && (input[start] == ' ' || input[start] == '\t'
        || input[start] == '\n' || input[start] == '\r')) start++;
    if (start >= input.size()) return "";
    size_t end = input.size() - 1;
    while (end > start && (input[end] == ' ' || input[end] == '\t'
        || input[end] == '\n' || input[end] == '\r')) end--;
    return input.substr(start, end - start + 1);
}

std::string formatCountToShortDecimal(int64_t value) {
    if (value < 0) return "-" + formatCountToShortDecimal(-value);
    if (value < 1000) return std::to_string(value);

    // Original: TreeMap of {1000:"k", 1000000:"M", 1000000000:"G"}
    struct Suffix { int64_t threshold; const char* suffix; };
    static const Suffix suffixes[] = {
        {1000000000, "G"},
        {1000000, "M"},
        {1000, "k"},
    };

    for (const auto& s : suffixes) {
        if (value >= s.threshold) {
            // Original: val truncated = value / (divideBy / 10)
            int64_t divideBy = s.threshold;
            int64_t truncated = value / (divideBy / 10);

            // Original: hasDecimal = truncated < 100 && truncated / 10.0 != (truncated / 10)
            bool hasDecimal = (truncated < 100) && ((truncated % 10) != 0);

            if (hasDecimal) {
                // Show one decimal: "1.2k", "3.4M"
                return std::to_string(truncated / 10) + "." + std::to_string(truncated % 10) + s.suffix;
            } else {
                // No decimal: "12k", "42M"
                return std::to_string(truncated / 10) + s.suffix;
            }
        }
    }

    return std::to_string(value);
}


int computeStringDistance(const std::string& a, const std::string& b, StringDistance algorithm) {
    if (algorithm == StringDistance::LEVENSHTEIN) {
        std::vector<std::vector<int>> dp(a.size()+1, std::vector<int>(b.size()+1));
        for (size_t i=0; i<=a.size(); i++) dp[i][0]=(int)i;
        for (size_t j=0; j<=b.size(); j++) dp[0][j]=(int)j;
        for (size_t i=1; i<=a.size(); i++)
            for (size_t j=1; j<=b.size(); j++)
                dp[i][j]=std::min({dp[i-1][j]+1,dp[i][j-1]+1,dp[i-1][j-1]+(a[i-1]!=b[j-1])});
        return dp[a.size()][b.size()];
    }
    return 0;
}
double computeStringSimilarity(const std::string& a, const std::string& b, StringDistance algorithm) {
    int dist = computeStringDistance(a, b, algorithm);
    int maxLen = (int)std::max(a.size(), b.size());
    return maxLen > 0 ? 1.0 - (double)dist / maxLen : 1.0;
}
std::string findClosestMatch(const std::string& target, const std::vector<std::string>& candidates, StringDistance algorithm) {
    std::string best; int bestDist = INT_MAX;
    for (const auto& c : candidates) {
        int d = computeStringDistance(target, c, algorithm);
        if (d < bestDist) { bestDist = d; best = c; }
    }
    return best;
}
std::string truncateString(const std::string& input, int maxLen, StringTruncation mode, const std::string& ellipsis) {
    if ((int)input.size() <= maxLen) return input;
    int keep = maxLen - (int)ellipsis.size();
    if (keep < 0) keep = 0;
    if (mode == StringTruncation::END) return input.substr(0, keep) + ellipsis;
    if (mode == StringTruncation::START) return ellipsis + input.substr(input.size()-keep);
    int half = keep / 2;
    return input.substr(0, half) + ellipsis + input.substr(input.size()-(keep-half));
}
std::vector<StringSearchResult> findAllOccurrences(const std::string& haystack, const std::string& needle) {
    std::vector<StringSearchResult> r; size_t p = 0;
    while ((p=haystack.find(needle,p)) != std::string::npos) {
        StringSearchResult sr; sr.found=true; sr.position=(int)p; sr.length=(int)needle.size(); r.push_back(sr);
        p += needle.size();
    }
    return r;
}
} // namespace pcp
