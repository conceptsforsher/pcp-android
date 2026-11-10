#include "pcp/user_directory_formatter.hpp"
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

std::string buildUserSearchBody(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"buildUserSearchBody"})";
    
    std::string type = parseJsonStringValue(json, "type");
    std::string content = parseJsonStringValue(json, "content");
    std::string format = parseJsonStringValue(json, "format");
    std::string eventType = parseJsonStringValue(json, "event_type");
    if (type.empty() && !eventType.empty()) type = eventType;
    if (type.empty()) type = "m.room.message";
    int64_t ts = parseJsonInt64Value(json, "timestamp", 0);
    
    std::ostringstream o;
    o << R"({"fn":")" << "buildUserSearchBody" << R"(","ok":true)";
    o << R"(,"type":")" << type << R"(")";
    o << R"(,"content_size":)" << content.size();
    if (!format.empty()) o << R"(,"format":")" << format << R"(")";
    if (ts > 0) o << R"(,"timestamp":)" << ts;
    o << R"(,"built":true)";
    o << "}";
    return o.str();
}

std::string formatUserDirectoryItem(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"formatUserDirectoryItem"})";
    
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
    o << R"({"fn":")" << "formatUserDirectoryItem" << R"(")";
    o << R"(,"len":)" << text.length();
    o << R"(,"words":)" << wordCount;
    o << R"(,"lines":)" << lineCount;
    if (wasTruncated) o << R"(,"truncated":true)";
    o << R"(,"display_len":)" << display.length();
    o << "}";
    return o.str();
}
