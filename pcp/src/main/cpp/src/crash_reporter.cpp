#include "pcp/crash_reporter.hpp"
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

std::string captureCrash(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"captureCrash"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "captureCrash" << R"(","ok":true)";
    o << R"(,"input_size":)" << input.size();
    if (!action.empty()) o << R"(,"action":")" << action << R"(")";
    if (!target.empty()) o << R"(,"target":")" << target << R"(")";
    if (flags != 0) o << R"(,"flags":)" << flags;
    o << R"(,"timeout_ms":)" << timeoutMs;
    if (async) o << R"(,"async":true)";
    o << R"(,"processed":true)";
    o << "}";
    return o.str();
}

std::string formatStackTrace(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"formatStackTrace"})";
    
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
    o << R"({"fn":")" << "formatStackTrace" << R"(")";
    o << R"(,"len":)" << text.length();
    o << R"(,"words":)" << wordCount;
    o << R"(,"lines":)" << lineCount;
    if (wasTruncated) o << R"(,"truncated":true)";
    o << R"(,"display_len":)" << display.length();
    o << "}";
    return o.str();
}

std::string getDeviceInfo(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getDeviceInfo" << R"(","ok":true)";
    
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

std::string buildCrashReport(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"buildCrashReport"})";
    
    std::string type = parseJsonStringValue(json, "type");
    std::string content = parseJsonStringValue(json, "content");
    std::string format = parseJsonStringValue(json, "format");
    std::string eventType = parseJsonStringValue(json, "event_type");
    if (type.empty() && !eventType.empty()) type = eventType;
    if (type.empty()) type = "m.room.message";
    int64_t ts = parseJsonInt64Value(json, "timestamp", 0);
    
    std::ostringstream o;
    o << R"({"fn":")" << "buildCrashReport" << R"(","ok":true)";
    o << R"(,"type":")" << type << R"(")";
    o << R"(,"content_size":)" << content.size();
    if (!format.empty()) o << R"(,"format":")" << format << R"(")";
    if (ts > 0) o << R"(,"timestamp":)" << ts;
    o << R"(,"built":true)";
    o << "}";
    return o.str();
}

std::string sendCrashReport(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"sendCrashReport"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "sendCrashReport" << R"(","ok":true)";
    o << R"(,"input_size":)" << input.size();
    if (!action.empty()) o << R"(,"action":")" << action << R"(")";
    if (!target.empty()) o << R"(,"target":")" << target << R"(")";
    if (flags != 0) o << R"(,"flags":)" << flags;
    o << R"(,"timeout_ms":)" << timeoutMs;
    if (async) o << R"(,"async":true)";
    o << R"(,"processed":true)";
    o << "}";
    return o.str();
}
