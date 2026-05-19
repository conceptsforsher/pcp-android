#ifndef PROGRESSIVE_STRING_UTILS_HPP
#define PROGRESSIVE_STRING_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// Remove characters that are invalid in Matrix room names.
// Keeps: letters, digits, spaces, common punctuation.
// Replaces: multiple spaces with single space, trims.
std::string sanitizeRoomName(const std::string& input);

// Replace various space characters (nbsp, thin space, etc.) with regular space.
std::string replaceSpaceChars(const std::string& input);

// Split a string by delimiter.
std::vector<std::string> splitString(const std::string& input, char delimiter);

// Join strings with delimiter.
std::string joinStrings(const std::vector<std::string>& parts, const std::string& delimiter);

// Trim whitespace from both ends.
std::string trim(const std::string& input);

// Check if string starts with prefix.
bool startsWith(const std::string& s, const std::string& prefix);

// Check if string ends with suffix.
bool endsWith(const std::string& s, const std::string& suffix);

// Convert to lowercase.
std::string toLower(const std::string& input);

// Convert to uppercase.
std::string toUpper(const std::string& input);

// Replace all occurrences of `from` with `to`.
std::string replaceAll(const std::string& input, const std::string& from, const std::string& to);

// Check if a string contains only digits.
bool isDigitsOnly(const std::string& input);

// Check if a string contains only letters.
bool isLettersOnly(const std::string& input);

// Check if a string is empty or whitespace only.
bool isBlank(const std::string& input);

// Remove HTML tags from a string.
std::string stripHtmlTags(const std::string& input);

// Count words in a string.
int wordCount(const std::string& input);

// Estimate reading time in seconds (200 WPM average).
int estimateReadingTimeSeconds(const std::string& input);

// Extract the first N words from a string.
std::string firstNWords(const std::string& input, int n);

// Format a count to short decimal: 42 → "42", 1234 → "1.2k", 12345 → "12k", 1.5M → "1.5M"
// Faithful port from im.vector.app.core.utils.TextUtils.kt (147L): formatCountToShortDecimal
// Original: TreeMap of {1000:"k", 1000000:"M", 1000000000:"G"} → floorEntry
std::string formatCountToShortDecimal(int64_t value);

} // namespace progressive

#endif // PROGRESSIVE_STRING_UTILS_HPP
