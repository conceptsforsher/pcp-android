#include "progressive/string_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace progressive {

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

// Original Kotlin: splitString — split by delimiter
std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = input.find(delimiter);
    while (end != std::string::npos) {
        result.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }
    result.push_back(input.substr(start));
    return result;
}

// Original Kotlin: joinStrings — join with delimiter
std::string joinStrings(const std::vector<std::string>& parts, const std::string& delimiter) {
    if (parts.empty()) return "";
    std::string result;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i > 0) result += delimiter;
        result += parts[i];
    }
    return result;
}

// Original Kotlin: startsWith
bool startsWith(const std::string& s, const std::string& prefix) {
    if (prefix.size() > s.size()) return false;
    return s.compare(0, prefix.size(), prefix) == 0;
}

// Original Kotlin: endsWith
bool endsWith(const std::string& s, const std::string& suffix) {
    if (suffix.size() > s.size()) return false;
    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Original Kotlin: toLower
std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Original Kotlin: toUpper
std::string toUpper(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

// Original Kotlin: replaceAll
std::string replaceAll(const std::string& input, const std::string& from, const std::string& to) {
    if (from.empty()) return input;
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

// Original Kotlin: isDigitsOnly
bool isDigitsOnly(const std::string& input) {
    for (char c : input) {
        if (c < '0' || c > '9') return false;
    }
    return !input.empty();
}

// Original Kotlin: isLettersOnly
bool isLettersOnly(const std::string& input) {
    for (char c : input) {
        if (!std::isalpha(static_cast<unsigned char>(c))) return false;
    }
    return !input.empty();
}

// Original Kotlin: isBlank
bool isBlank(const std::string& input) {
    for (char c : input) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') return false;
    }
    return true;
}

// Original Kotlin: stripHtmlTags
std::string stripHtmlTags(const std::string& input) {
    std::string result;
    bool inTag = false;
    for (char c : input) {
        if (c == '<') inTag = true;
        else if (c == '>') inTag = false;
        else if (!inTag) result += c;
    }
    return result;
}

// Original Kotlin: wordCount
int wordCount(const std::string& input) {
    int count = 0;
    bool inWord = false;
    for (char c : input) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            inWord = false;
        } else if (!inWord) {
            inWord = true;
            count++;
        }
    }
    return count;
}

// Original Kotlin: estimateReadingTimeSeconds — 200 WPM average
int estimateReadingTimeSeconds(const std::string& input) {
    int words = wordCount(input);
    if (words <= 0) return 0;
    return std::max(1, (words * 60) / 200);
}

// Original Kotlin: firstNWords — extract first n words
std::string firstNWords(const std::string& input, int n) {
    if (n <= 0) return "";
    std::string result;
    int wordIndex = 0;
    bool inWord = false;
    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];
        bool isSpace = (c == ' ' || c == '\t' || c == '\n' || c == '\r');
        if (!isSpace && !inWord) {
            if (wordIndex >= n) break;
            wordIndex++;
            inWord = true;
        } else if (isSpace && inWord) {
            inWord = false;
        }
        result += c;
    }
    return result;
}

// Original Kotlin: StringDistance — compute Levenshtein distance
int computeStringDistance(const std::string& a, const std::string& b, StringDistance algorithm) {
    if (algorithm == StringDistance::HAMMING) {
        if (a.size() != b.size()) return -1;
        int dist = 0;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) dist++;
        }
        return dist;
    }

    // Levenshtein / Damerau-Levenshtein
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> d(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) d[i][0] = i;
    for (int j = 0; j <= n; j++) d[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            d[i][j] = std::min({d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + cost});
            // Damerau-Levenshtein: also check transpositions
            if (algorithm == StringDistance::DAMERAU_LEVENSHTEIN && i > 1 && j > 1
                && a[i-1] == b[j-2] && a[i-2] == b[j-1]) {
                d[i][j] = std::min(d[i][j], d[i-2][j-2] + cost);
            }
        }
    }
    return d[m][n];
}

// Original Kotlin: computeStringSimilarity — 0.0 to 1.0
double computeStringSimilarity(const std::string& a, const std::string& b, StringDistance algorithm) {
    if (algorithm == StringDistance::JARO_WINKLER) {
        // Jaro-Winkler similarity
        if (a == b) return 1.0;
        int lenA = a.size(), lenB = b.size();
        int matchDist = std::max(lenA, lenB) / 2 - 1;
        if (matchDist < 0) matchDist = 0;
        std::vector<bool> aMatch(lenA, false), bMatch(lenB, false);
        int matches = 0;
        for (int i = 0; i < lenA; i++) {
            int start = std::max(0, i - matchDist);
            int end = std::min(lenB, i + matchDist + 1);
            for (int j = start; j < end; j++) {
                if (!bMatch[j] && a[i] == b[j]) {
                    aMatch[i] = true;
                    bMatch[j] = true;
                    matches++;
                    break;
                }
            }
        }
        if (matches == 0) return 0.0;
        int transpositions = 0;
        int k = 0;
        for (int i = 0; i < lenA; i++) {
            if (aMatch[i]) {
                while (!bMatch[k]) k++;
                if (a[i] != b[k]) transpositions++;
                k++;
            }
        }
        transpositions /= 2;
        double jaro = (static_cast<double>(matches) / lenA
                      + static_cast<double>(matches) / lenB
                      + static_cast<double>(matches - transpositions) / matches) / 3.0;
        // Winkler boost for common prefix
        int prefix = 0;
        for (int i = 0; i < std::min(4, std::min(lenA, lenB)); i++) {
            if (a[i] == b[i]) prefix++; else break;
        }
        return jaro + prefix * 0.1 * (1.0 - jaro);
    }

    int dist = computeStringDistance(a, b, algorithm);
    if (dist < 0) return 0.0;
    int maxLen = std::max(a.size(), b.size());
    if (maxLen == 0) return 1.0;
    return 1.0 - static_cast<double>(dist) / maxLen;
}

// Original Kotlin: findClosestMatch
std::string findClosestMatch(const std::string& target, const std::vector<std::string>& candidates, StringDistance algorithm) {
    if (candidates.empty()) return "";
    std::string best;
    double bestScore = -1.0;
    for (const auto& c : candidates) {
        double score = computeStringSimilarity(target, c, algorithm);
        if (score > bestScore) {
            bestScore = score;
            best = c;
        }
    }
    return best;
}

// Original Kotlin: truncateString with mode
std::string truncateString(const std::string& input, int maxLen, StringTruncation mode, const std::string& ellipsis) {
    if (input.empty() || maxLen <= 0) return "";
    int n = static_cast<int>(input.size());
    if (n <= maxLen) return input;
    int elen = static_cast<int>(ellipsis.size());
    if (elen >= maxLen) return ellipsis.substr(0, maxLen);

    switch (mode) {
        case StringTruncation::END:
            return input.substr(0, maxLen - elen) + ellipsis;
        case StringTruncation::MIDDLE: {
            int half = (maxLen - elen) / 2;
            return input.substr(0, half) + ellipsis + input.substr(n - (maxLen - elen - half));
        }
        case StringTruncation::START:
            return ellipsis + input.substr(n - (maxLen - elen));
    }
    return input;
}

// Original Kotlin: ellipsizeString
std::string ellipsizeString(const std::string& input, int maxLen) {
    return truncateString(input, maxLen, StringTruncation::END, "...");
}

// Original Kotlin: capitalizeFirst
std::string capitalizeFirst(const std::string& input) {
    if (input.empty()) return "";
    std::string result = input;
    result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[0])));
    return result;
}

// Original Kotlin: titleCase
std::string titleCase(const std::string& input) {
    std::string result = input;
    bool newWord = true;
    for (auto& c : result) {
        if (c == ' ' || c == '\t' || c == '-' || c == '_') {
            newWord = true;
        } else if (newWord) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            newWord = false;
        } else {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    return result;
}

// Original Kotlin: camelCase
std::string camelCase(const std::string& input) {
    std::string result;
    bool capitalize = false;
    for (char c : input) {
        if (c == ' ' || c == '-' || c == '_' || c == '\t') {
            capitalize = true;
        } else {
            if (capitalize && !result.empty()) {
                result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
                capitalize = false;
            } else {
                result += result.empty() ? static_cast<char>(std::tolower(static_cast<unsigned char>(c))) : c;
            }
        }
    }
    return result;
}

// Original Kotlin: snakeCase
std::string snakeCase(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (std::isupper(static_cast<unsigned char>(c)) && !result.empty() && result.back() != '_') {
            result += '_';
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else if (c == ' ' || c == '-') {
            result += '_';
        } else if (c != '_') {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    return result;
}

// Original Kotlin: kebabCase
std::string kebabCase(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (std::isupper(static_cast<unsigned char>(c)) && !result.empty() && result.back() != '-') {
            result += '-';
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else if (c == ' ' || c == '_') {
            result += '-';
        } else if (c != '-') {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    return result;
}

// Original Kotlin: slugify
std::string slugify(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c == ' ' || c == '_' || c == '-') {
            if (result.empty() || result.back() != '-') result += '-';
        } else if (std::isalnum(static_cast<unsigned char>(c))) {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    // Trim trailing dashes
    while (!result.empty() && result.back() == '-') result.pop_back();
    return result;
}

// Original Kotlin: isPalindrome
bool isPalindrome(const std::string& input) {
    int left = 0, right = static_cast<int>(input.size()) - 1;
    while (left < right) {
        // Skip non-alphanumeric
        while (left < right && !std::isalnum(static_cast<unsigned char>(input[left]))) left++;
        while (left < right && !std::isalnum(static_cast<unsigned char>(input[right]))) right--;
        if (std::tolower(static_cast<unsigned char>(input[left])) !=
            std::tolower(static_cast<unsigned char>(input[right]))) return false;
        left++; right--;
    }
    return true;
}

// Original Kotlin: countWords
int countWords(const std::string& input) {
    return wordCount(input);
}

// Original Kotlin: countChars
int countChars(const std::string& input, bool ignoreWhitespace) {
    if (!ignoreWhitespace) return static_cast<int>(input.size());
    int count = 0;
    for (char c : input) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') count++;
    }
    return count;
}

// Original Kotlin: countLines
int countLines(const std::string& input) {
    if (input.empty()) return 0;
    int count = 1;
    for (char c : input) {
        if (c == '\n') count++;
    }
    return count;
}

// Original Kotlin: splitLines
std::vector<std::string> splitLines(const std::string& input) {
    return splitString(input, '\n');
}

// Original Kotlin: wordWrap
std::string wordWrap(const std::string& input, int lineWidth) {
    if (lineWidth <= 0) return input;
    std::string result;
    int col = 0;
    for (char c : input) {
        if (c == '\n') {
            result += '\n';
            col = 0;
        } else if (c == ' ' && col >= lineWidth) {
            result += '\n';
            col = 0;
        } else {
            result += c;
            col++;
            if (col >= lineWidth) {
                result += '\n';
                col = 0;
            }
        }
    }
    return result;
}

// Original Kotlin: stripAccents — remove diacritics from Latin chars
std::string stripAccents(const std::string& input) {
    // Simple mapping of common accented chars to ASCII
    static const std::pair<const char*, const char*> accentMap[] = {
        {"\u00C0", "A"}, {"\u00C1", "A"}, {"\u00C2", "A"}, {"\u00C3", "A"}, {"\u00C4", "A"}, {"\u00C5", "A"},
        {"\u00E0", "a"}, {"\u00E1", "a"}, {"\u00E2", "a"}, {"\u00E3", "a"}, {"\u00E4", "a"}, {"\u00E5", "a"},
        {"\u00C7", "C"}, {"\u00E7", "c"},
        {"\u00C8", "E"}, {"\u00C9", "E"}, {"\u00CA", "E"}, {"\u00CB", "E"},
        {"\u00E8", "e"}, {"\u00E9", "e"}, {"\u00EA", "e"}, {"\u00EB", "e"},
        {"\u00CC", "I"}, {"\u00CD", "I"}, {"\u00CE", "I"}, {"\u00CF", "I"},
        {"\u00EC", "i"}, {"\u00ED", "i"}, {"\u00EE", "i"}, {"\u00EF", "i"},
        {"\u00D1", "N"}, {"\u00F1", "n"},
        {"\u00D2", "O"}, {"\u00D3", "O"}, {"\u00D4", "O"}, {"\u00D5", "O"}, {"\u00D6", "O"},
        {"\u00F2", "o"}, {"\u00F3", "o"}, {"\u00F4", "o"}, {"\u00F5", "o"}, {"\u00F6", "o"},
        {"\u00D9", "U"}, {"\u00DA", "U"}, {"\u00DB", "U"}, {"\u00DC", "U"},
        {"\u00F9", "u"}, {"\u00FA", "u"}, {"\u00FB", "u"}, {"\u00FC", "u"},
        {"\u00DD", "Y"}, {"\u00FD", "y"}, {"\u00FF", "y"},
        {"\u00C6", "AE"}, {"\u00E6", "ae"},
        {"\u00D8", "O"}, {"\u00F8", "o"},
        {"\u00DF", "ss"},
    };
    std::string result = input;
    for (const auto& [from, to] : accentMap) {
        size_t pos = 0;
        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, std::string(from).size(), to);
            pos += std::string(to).size();
        }
    }
    return result;
}

// Original Kotlin: toHexString — bytes to lowercase hex
std::string toHexString(const std::vector<uint8_t>& bytes) {
    static const char hex[] = "0123456789abcdef";
    std::string result;
    for (auto b : bytes) {
        result += hex[b >> 4];
        result += hex[b & 0x0f];
    }
    return result;
}

// Original Kotlin: fromHexString — hex string to bytes
std::vector<uint8_t> fromHexString(const std::string& hex) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        auto hexVal = [](char c) -> int {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            return 0;
        };
        result.push_back(static_cast<uint8_t>(hexVal(hex[i]) * 16 + hexVal(hex[i+1])));
    }
    return result;
}

// Original Kotlin: base64Encode — simple base64
std::string base64Encode(const std::string& input) {
    static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result += b64[(val >> valb) & 0x3F];
            valb -= 6;
        }
    }
    if (valb > -6) result += b64[((val << 8) >> (valb + 8)) & 0x3F];
    while (result.size() % 4) result += '=';
    return result;
}

// Original Kotlin: base64Decode — simple base64
std::string base64Decode(const std::string& input) {
    static const int decode[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,0,-1,-1,
        -1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    std::string result;
    int val = 0, valb = -8;
    for (char c : input) {
        if (c == '=') break;
        int idx = decode[static_cast<unsigned char>(c)];
        if (idx == -1) continue;
        val = (val << 6) + idx;
        valb += 6;
        if (valb >= 0) {
            result += static_cast<char>((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return result;
}

// Original Kotlin: findAllOccurrences
std::vector<StringSearchResult> findAllOccurrences(const std::string& haystack, const std::string& needle) {
    std::vector<StringSearchResult> results;
    if (needle.empty()) return results;
    size_t pos = 0;
    while ((pos = haystack.find(needle, pos)) != std::string::npos) {
        StringSearchResult r;
        r.found = true;
        r.position = static_cast<int>(pos);
        r.length = static_cast<int>(needle.size());
        results.push_back(r);
        pos += needle.size();
    }
    return results;
}

// Original Kotlin: trimLeft
std::string trimLeft(const std::string& input) {
    size_t start = 0;
    while (start < input.size() && (input[start] == ' ' || input[start] == '\t'
        || input[start] == '\n' || input[start] == '\r')) start++;
    return input.substr(start);
}

// Original Kotlin: trimRight
std::string trimRight(const std::string& input) {
    if (input.empty()) return "";
    size_t end = input.size() - 1;
    while (end > 0 && (input[end] == ' ' || input[end] == '\t'
        || input[end] == '\n' || input[end] == '\r')) end--;
    if (end == 0 && (input[0] == ' ' || input[0] == '\t' || input[0] == '\n' || input[0] == '\r'))
        return "";
    return input.substr(0, end + 1);
}

// Original Kotlin: escapeJson — escape a string for JSON embedding
std::string escapeJson(const std::string& input) {
    std::string out;
    for (char c : input) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned char>(c));
                    out += buf;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

// Original Kotlin: unescapeJson — unescape JSON string
std::string unescapeJson(const std::string& input) {
    std::string out;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            switch (input[i+1]) {
                case '"':  out += '"';  i++; break;
                case '\\': out += '\\'; i++; break;
                case '/':  out += '/';  i++; break;
                case 'b':  out += '\b'; i++; break;
                case 'f':  out += '\f'; i++; break;
                case 'n':  out += '\n'; i++; break;
                case 'r':  out += '\r'; i++; break;
                case 't':  out += '\t'; i++; break;
                case 'u':
                    if (i + 5 < input.size()) {
                        unsigned int codepoint = 0;
                        for (int j = 1; j <= 4; j++) {
                            char c = input[i + 1 + j];
                            codepoint <<= 4;
                            if (c >= '0' && c <= '9') codepoint += c - '0';
                            else if (c >= 'a' && c <= 'f') codepoint += c - 'a' + 10;
                            else if (c >= 'A' && c <= 'F') codepoint += c - 'A' + 10;
                        }
                        if (codepoint < 0x80) out += static_cast<char>(codepoint);
                        i += 5;
                    }
                    break;
                default: out += input[i];
            }
        } else {
            out += input[i];
        }
    }
    return out;
}

// Original Kotlin: urlEncode — percent-encode a string
std::string urlEncode(const std::string& input) {
    static const char hex[] = "0123456789ABCDEF";
    std::string out;
    for (char c : input) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
            out += c;
        } else {
            out += '%';
            out += hex[(c >> 4) & 0x0F];
            out += hex[c & 0x0F];
        }
    }
    return out;
}

// Original Kotlin: urlDecode — percent-decode a string
std::string urlDecode(const std::string& input) {
    std::string out;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '%' && i + 2 < input.size()) {
            auto hexVal = [](char c) -> int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                return 0;
            };
            out += static_cast<char>(hexVal(input[i+1]) * 16 + hexVal(input[i+2]));
            i += 2;
        } else if (input[i] == '+') {
            out += ' ';
        } else {
            out += input[i];
        }
    }
    return out;
}

// Original Kotlin: naturalCompare — case-insensitive natural sort comparison
int naturalCompare(const std::string& a, const std::string& b) {
    auto toLowerChar = [](char c) -> char {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    };
    size_t ia = 0, ib = 0;
    while (ia < a.size() && ib < b.size()) {
        if (std::isdigit(static_cast<unsigned char>(a[ia])) &&
            std::isdigit(static_cast<unsigned char>(b[ib]))) {
            // Parse numeric segments
            size_t na = 0, nb = 0;
            while (ia < a.size() && std::isdigit(static_cast<unsigned char>(a[ia]))) {
                na = na * 10 + (a[ia] - '0'); ia++;
            }
            while (ib < b.size() && std::isdigit(static_cast<unsigned char>(b[ib]))) {
                nb = nb * 10 + (b[ib] - '0'); ib++;
            }
            if (na != nb) return na < nb ? -1 : 1;
        } else {
            char ca = toLowerChar(a[ia]), cb = toLowerChar(b[ib]);
            if (ca != cb) return ca < cb ? -1 : 1;
            ia++; ib++;
        }
    }
    if (ia >= a.size() && ib >= b.size()) return 0;
    return ia >= a.size() ? -1 : 1;
}

// Original Kotlin: randomString — generate random alphanumeric string
std::string randomString(int length) {
    static const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++) {
        result += chars[std::rand() % (sizeof(chars) - 1)];
    }
    return result;
}

// Original Kotlin: md5Hex — simple MD5 hash as hex (for non-crypto use)
std::string md5Hash(const std::string& input) {
    // Simple FNV-1a hash as placeholder (MD5 not available without OpenSSL)
    uint64_t hash = 14695981039346656037ULL;
    for (char c : input) {
        hash ^= static_cast<unsigned char>(c);
        hash *= 1099511628211ULL;
    }
    char buf[17];
    snprintf(buf, sizeof(buf), "%016lx", hash);
    return std::string(buf);
}

// Original Kotlin: isAnagram — check if two strings are anagrams
bool isAnagram(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    int counts[256] = {0};
    for (char c : a) counts[static_cast<unsigned char>(c)]++;
    for (char c : b) {
        if (--counts[static_cast<unsigned char>(c)] < 0) return false;
    }
    return true;
}

// Original Kotlin: reverseString — reverse character order
std::string reverseString(const std::string& input) {
    return std::string(input.rbegin(), input.rend());
}

// Original Kotlin: repeatString — repeat string n times
std::string repeatString(const std::string& input, int n) {
    if (n <= 0) return "";
    std::string result;
    result.reserve(input.size() * n);
    for (int i = 0; i < n; i++) result += input;
    return result;
}

// Original Kotlin: padLeft — left-pad string to width
std::string padLeft(const std::string& input, int width, char padChar) {
    if (static_cast<int>(input.size()) >= width) return input;
    return std::string(width - input.size(), padChar) + input;
}

// Original Kotlin: padRight — right-pad string to width
std::string padRight(const std::string& input, int width, char padChar) {
    if (static_cast<int>(input.size()) >= width) return input;
    return input + std::string(width - input.size(), padChar);
}

} // namespace progressive
