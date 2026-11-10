#include "pcp/event_highlight_display.hpp"
#include "pcp/json_parser.hpp"
#include <string>
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <vector>
#include <map>

using pcp::parseJsonStringValue;
using pcp::parseJsonBoolValue;
using pcp::parseJsonInt64Value;

std::string getHighlightColor(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getHighlightColor" << R"(","ok":true)";
    
    if (!json.empty()) {
        std::string key = parseJsonStringValue(json, "key");
        std::string id = parseJsonStringValue(json, "id");
        std::string query = parseJsonStringValue(json, "query");
        int64_t limit = parseJsonInt64Value(json, "limit", 50);
        int64_t offset = parseJsonInt64Value(json, "offset", 0);
        bool includeDeleted = parseJsonBoolValue(json, "include_deleted", false);
        
        if (!key.empty()) o << R"(,"key":")" << key << R"(")";
        if (!id.empty()) o << R"(,"id":")" << id << R"(")";
        if (!query.empty()) o << R"(,"query":")" << query << R"(")";
        o << R"(,"limit":)" << limit;
        o << R"(,"offset":)" << offset;
        if (includeDeleted) o << R"(,"include_deleted":true)";
        o << R"(,"input_size":)" << json.size();
    }
    o << R"(,"total":0,"items":[])";
    o << "}";
    return o.str();
}

std::string formatHighlightBadge(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"formatHighlightBadge"})";
    
    std::string text = parseJsonStringValue(json, "text");
    if (text.empty()) text = parseJsonStringValue(json, "body");
    if (text.empty()) text = parseJsonStringValue(json, "value");
    if (text.empty()) text = json;
    int64_t maxLen = parseJsonInt64Value(json, "maxLength", 1024);
    bool truncate = parseJsonBoolValue(json, "truncate", true);
    
    std::string display = text;
    bool wasTruncated = false;
    if (truncate && display.length() > static_cast<size_t>(maxLen)) {
        display = display.substr(0, static_cast<size_t>(maxLen));
        size_t lastSpace = display.rfind(' ');
        if (lastSpace != std::string::npos && lastSpace > static_cast<size_t>(maxLen) / 2)
            display = display.substr(0, lastSpace);
        display += "...";
        wasTruncated = true;
    }
    
    int wordCount = 0, lineCount = 1;
    bool inWord = false;
    for (char c : text) {
        if (c == '\n') lineCount++;
        if (std::isspace(static_cast<unsigned char>(c))) inWord = false;
        else { if (!inWord) { wordCount++; inWord = true; } }
    }
    
    std::ostringstream o;
    o << R"({"fn":")" << "formatHighlightBadge" << R"(")";
    o << R"(,"len":)" << text.length();
    o << R"(,"words":)" << wordCount;
    o << R"(,"lines":)" << lineCount;
    if (wasTruncated) o << R"(,"truncated":true)";
    o << R"(,"display_len":)" << display.length();
    o << "}";
    return o.str();
}

std::string getHighlightSoundHint(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getHighlightSoundHint" << R"(","ok":true)";
    
    if (!json.empty()) {
        std::string key = parseJsonStringValue(json, "key");
        std::string id = parseJsonStringValue(json, "id");
        std::string query = parseJsonStringValue(json, "query");
        int64_t limit = parseJsonInt64Value(json, "limit", 50);
        int64_t offset = parseJsonInt64Value(json, "offset", 0);
        bool includeDeleted = parseJsonBoolValue(json, "include_deleted", false);
        
        if (!key.empty()) o << R"(,"key":")" << key << R"(")";
        if (!id.empty()) o << R"(,"id":")" << id << R"(")";
        if (!query.empty()) o << R"(,"query":")" << query << R"(")";
        o << R"(,"limit":)" << limit;
        o << R"(,"offset":)" << offset;
        if (includeDeleted) o << R"(,"include_deleted":true)";
        o << R"(,"input_size":)" << json.size();
    }
    o << R"(,"total":0,"items":[])";
    o << "}";
    return o.str();
}
