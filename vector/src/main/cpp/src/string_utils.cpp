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

} // namespace progressive
