#include "progressive/text_formats.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

std::string escapeHtml(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (char c : text) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            default:  out += c;
        }
    }
    return out;
}

std::string formatSpoiler(const std::string& text) {
    return "<span data-mx-spoiler>" + text + "</span>";
}

std::string formatEmote(const std::string& senderName, const std::string& text) {
    return "<em>" + escapeHtml(senderName) + " " + escapeHtml(text) + "</em>";
}

std::string formatShrug(const std::string& text) {
    if (text.empty()) return "\u00AF\\_(\u30C4)_/\u00AF"; // ¯\_(ツ)_/¯
    return text + " \u00AF\\_(\u30C4)_/\u00AF";
}

std::string formatLenny(const std::string& text) {
    if (text.empty()) return "(\u0361\u00B0 \u035C\u0296 \u0361\u00B0)"; // ( ͡° ͜ʖ ͡°)
    return text + " (\u0361\u00B0 \u035C\u0296 \u0361\u00B0)";
}

std::string formatTableFlip(const std::string& text) {
    if (text.empty()) return "(\u256F\u00B0\u25A1\u00B0)\u256F\uFE35 \u253B\u2501\u253B"; // (╯°□°）╯︵ ┻━┻
    return text + " (\u256F\u00B0\u25A1\u00B0)\u256F\uFE35 \u253B\u2501\u253B";
}

std::string formatPlain(const std::string& text) {
    // Strip markdown formatting: **bold**, *italic*, `code`, etc.
    std::string result = text;
    // Remove bold markers
    size_t pos = 0;
    while ((pos = result.find("**")) != std::string::npos) {
        result.erase(pos, 2);
        auto end = result.find("**", pos);
        if (end != std::string::npos) result.erase(end, 2);
    }
    // Remove italic markers
    while ((pos = result.find("__")) != std::string::npos) {
        result.erase(pos, 2);
        auto end = result.find("__", pos);
        if (end != std::string::npos) result.erase(end, 2);
    }
    return result;
}

std::string truncateText(const std::string& text, int maxLen) {
    if (maxLen <= 0) return {};
    int count = utf8CharCount(text);
    if (count <= maxLen) return text;

    // Walk through UTF-8 and stop at maxLen characters
    int chars = 0;
    size_t byteLen = 0;
    for (size_t i = 0; i < text.size() && chars < maxLen; ++i) {
        unsigned char c = text[i];
        if (c < 0x80 || c >= 0xC0) chars++;
        byteLen = i + 1;
    }
    return text.substr(0, byteLen) + "...";
}

int utf8CharCount(const std::string& text) {
    int count = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = text[i];
        if (c < 0x80 || c >= 0xC0) count++;
    }
    return count;
}

bool isEmojiOnly(const std::string& text) {
    if (text.empty()) return false;
    for (size_t i = 0; i < text.size(); ++i) {
        unsigned char c = text[i];
        // Skip whitespace
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
        // Emoji range: high Unicode (U+1F600-U+1F64F, etc.)
        if (c >= 0xF0) {
            // 4-byte UTF-8, skip 3 continuation bytes
            i += 3;
            continue;
        }
        // Some emoji are in 3-byte range (U+2600-U+27BF, U+2B50, etc.)
        if (c == 0xE2) {
            if (i + 2 < text.size()) {
                unsigned char c1 = text[i+1];
                unsigned char c2 = text[i+2];
                // Misc symbols, dingbats
                if ((c1 == 0x9A || c1 == 0x9C || c1 == 0xAC || c1 == 0xAD) ||
                    (c1 == 0x98 && c2 >= 0x80) || (c1 == 0x99 && c2 <= 0xBF) ||
                    (c1 == 0x9D && c2 >= 0x80)) {
                    i += 2;
                    continue;
                }
            }
        }
        // ASCII character found — not emoji-only
        return false;
    }
    return true;
}



} // namespace progressive
