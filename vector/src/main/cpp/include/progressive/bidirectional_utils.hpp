#pragma once

#include <string>
#include <cstdint>
#include <algorithm>

namespace progressive {

// ================================================================
// Unicode Bidi (Bidirectional) Text Security Utilities
//
// Protects against Unicode bidi override attacks (CVE-2021-42574).
// Attackers can insert RTL/LTR override characters into URLs or
// filenames to misrepresent their true content.
//
// Example attack:
//   "https://evil.com/\u202Eﬁle良性.exe" displays as
//   "https://evil.com/exe.良性ﬁle" — hiding the real file extension.
//
// Mitigation: detect and strip bidirectional override characters
// before displaying user-generated content.
//
// Ported from Element Android:
//   im.vector.app.core.extensions.String.kt (lines 10-17)
//
// Unicode characters:
//   U+202D  LEFT-TO-RIGHT OVERRIDE  (LRO)
//   U+202E  RIGHT-TO-LEFT OVERRIDE  (RLO)
//   U+202A  LEFT-TO-RIGHT EMBEDDING (LRE)
//   U+202B  RIGHT-TO-LEFT EMBEDDING (RLE)
//   U+202C  POP DIRECTIONAL FORMATTING (PDF)
//   U+2066  LEFT-TO-RIGHT ISOLATE    (LRI)
//   U+2067  RIGHT-TO-LEFT ISOLATE   (RLI)
//   U+2068  FIRST STRONG ISOLATE    (FSI)
//   U+2069  POP DIRECTIONAL ISOLATE (PDI)
// ================================================================

// ---- Constant Characters ----

constexpr char16_t CHAR_LRO = 0x202D; // LEFT-TO-RIGHT OVERRIDE
constexpr char16_t CHAR_RLO = 0x202E; // RIGHT-TO-LEFT OVERRIDE
constexpr char16_t CHAR_LRE = 0x202A; // LEFT-TO-RIGHT EMBEDDING
constexpr char16_t CHAR_RLE = 0x202B; // RIGHT-TO-LEFT EMBEDDING
constexpr char16_t CHAR_PDF = 0x202C; // POP DIRECTIONAL FORMATTING
constexpr char16_t CHAR_LRI = 0x2066; // LEFT-TO-RIGHT ISOLATE
constexpr char16_t CHAR_RLI = 0x2067; // RIGHT-TO-LEFT ISOLATE
constexpr char16_t CHAR_FSI = 0x2068; // FIRST STRONG ISOLATE
constexpr char16_t CHAR_PDI = 0x2069; // POP DIRECTIONAL ISOLATE

// ---- UTF-8 Byte Sequences for Override Characters ----

constexpr const char* UTF8_LRO = "\xE2\x80\xAD"; // U+202D in UTF-8 (3 bytes)
constexpr const char* UTF8_RLO = "\xE2\x80\xAE"; // U+202E in UTF-8 (3 bytes)

// ---- Detection ----

// Check if a char16_t is a bidirectional override character.
inline bool isBidiOverrideChar(char16_t c) {
    return c == CHAR_LRO || c == CHAR_RLO ||
           c == CHAR_LRE || c == CHAR_RLE ||
           c == CHAR_PDF || c == CHAR_LRI ||
           c == CHAR_RLI || c == CHAR_FSI ||
           c == CHAR_PDI;
}

// Check if a string contains any bidirectional override characters.
// Scans for the UTF-8 byte sequences of U+202A through U+202E
// and U+2066 through U+2069.
inline bool containsBidiOverride(const std::string& str) {
    for (size_t i = 0; i + 2 < str.size(); i++) {
        unsigned char b0 = static_cast<unsigned char>(str[i]);
        unsigned char b1 = static_cast<unsigned char>(str[i + 1]);
        unsigned char b2 = static_cast<unsigned char>(str[i + 2]);
        // U+202A..U+202E → E2 80 AA..AE
        if (b0 == 0xE2 && b1 == 0x80 && b2 >= 0xAA && b2 <= 0xAE)
            return true;
        // U+2066..U+2069 → E2 81 A6..A9
        if (b0 == 0xE2 && b1 == 0x81 && b2 >= 0xA6 && b2 <= 0xA9)
            return true;
    }
    return false;
}

// Check if a string contains a RIGHT-TO-LEFT OVERRIDE character (U+202E).
// This is the most common attack vector — RLO reverses display direction.
inline bool containsRtlOverride(const std::string& str) {
    for (size_t i = 0; i + 2 < str.size(); i++) {
        unsigned char b0 = static_cast<unsigned char>(str[i]);
        unsigned char b1 = static_cast<unsigned char>(str[i + 1]);
        unsigned char b2 = static_cast<unsigned char>(str[i + 2]);
        if (b0 == 0xE2 && b1 == 0x80 && b2 == 0xAE)
            return true;
    }
    return false;
}

// ---- Filtering ----

// Remove all bidirectional override characters from a string.
// Scans for U+202A..U+202E and U+2066..U+2069 UTF-8 sequences.
// Returns a new string without any bidi controls.
inline std::string filterBidiOverrides(const std::string& str) {
    if (!containsBidiOverride(str)) return str;
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ) {
        size_t remaining = str.size() - i;
        if (remaining >= 3) {
            unsigned char b0 = static_cast<unsigned char>(str[i]);
            unsigned char b1 = static_cast<unsigned char>(str[i + 1]);
            unsigned char b2 = static_cast<unsigned char>(str[i + 2]);
            bool isBidi = false;
            if (b0 == 0xE2 && b1 == 0x80 && b2 >= 0xAA && b2 <= 0xAE)
                isBidi = true;
            if (b0 == 0xE2 && b1 == 0x81 && b2 >= 0xA6 && b2 <= 0xA9)
                isBidi = true;
            if (isBidi) {
                i += 3;
                continue;
            }
        }
        result += str[i];
        i++;
    }
    return result;
}

// ---- Display Sanitization ----

// Sanitize text for safe display:
// 1. Filter out all bidi override characters
// 2. If the original contained RTL overrides, append LTR mark (U+200E)
//    to force correct display direction at string end.
// This is the combined equivalent of:
//   str.filterDirectionOverrides().ensureEndsLeftToRight()
inline std::string sanitizeDisplayText(const std::string& str) {
    bool hadRtl = containsRtlOverride(str);
    auto cleaned = filterBidiOverrides(str);
    if (hadRtl) {
        // Append U+200E LEFT-TO-RIGHT MARK (UTF-8: E2 80 8E)
        cleaned += "\xE2\x80\x8E";
    }
    return cleaned;
}

} // namespace progressive
