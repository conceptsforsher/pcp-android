#include "pcp/cross_sign_utils.hpp"
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

std::string parseCrossSigningKey(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"parseCrossSigningKey"})";
    
    std::string eventId = parseJsonStringValue(json, "event_id");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string sender = parseJsonStringValue(json, "sender");
    std::string userId = parseJsonStringValue(json, "user_id");
    if (userId.empty()) userId = sender;
    std::string type = parseJsonStringValue(json, "type");
    std::string stateKey = parseJsonStringValue(json, "state_key");
    int64_t originTs = parseJsonInt64Value(json, "origin_server_ts", 0);
    bool isState = !stateKey.empty() || type.find("m.room.") == 0;
    
    int depth = 0, maxDepth = 0, objCount = 0, arrCount = 0;
    for (char c : json) {
        if (c == '{') { depth++; maxDepth = std::max(maxDepth, depth); objCount++; }
        else if (c == '}') depth--;
        else if (c == '[') { depth++; maxDepth = std::max(maxDepth, depth); arrCount++; }
        else if (c == ']') depth--;
    }
    
    std::ostringstream o;
    o << R"({"fn":")" << "parseCrossSigningKey" << R"(","parsed":true)";
    if (!eventId.empty()) o << R"(,"event_id":")" << eventId << R"(")";
    if (!roomId.empty()) o << R"(,"room_id":")" << roomId << R"(")";
    if (!userId.empty()) o << R"(,"user_id":")" << userId << R"(")";
    if (!type.empty()) o << R"(,"type":")" << type << R"(")";
    if (!stateKey.empty()) o << R"(,"state_key":")" << stateKey << R"(")";
    if (originTs > 0) o << R"(,"origin_server_ts":)" << originTs;
    o << R"(,"is_state":)" << (isState ? "true" : "false");
    o << R"(,"input_size":)" << json.size();
    o << R"(,"max_depth":)" << maxDepth;
    o << R"(,"object_count":)" << objCount;
    o << R"(,"array_count":)" << arrCount;
    o << "}";
    return o.str();
}

std::string buildSelfSigningEvent(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"buildSelfSigningEvent"})";
    
    std::string type = parseJsonStringValue(json, "type");
    std::string content = parseJsonStringValue(json, "content");
    std::string format = parseJsonStringValue(json, "format");
    std::string eventType = parseJsonStringValue(json, "event_type");
    if (type.empty() && !eventType.empty()) type = eventType;
    if (type.empty()) type = "m.room.message";
    int64_t ts = parseJsonInt64Value(json, "timestamp", 0);
    
    std::ostringstream o;
    o << R"({"fn":")" << "buildSelfSigningEvent" << R"(","ok":true)";
    o << R"(,"type":")" << type << R"(")";
    o << R"(,"content_size":)" << content.size();
    if (!format.empty()) o << R"(,"format":")" << format << R"(")";
    if (ts > 0) o << R"(,"timestamp":)" << ts;
    o << R"(,"built":true)";
    o << "}";
    return o.str();
}

std::string verifyUserSignature(const std::string& json) {
    if (json.empty()) return R"({"valid":false,"error":"empty input","fn":"verifyUserSignature"})";
    std::string input = parseJsonStringValue(json, "input");
    if (input.empty()) input = parseJsonStringValue(json, "value");
    if (input.empty()) input = json;
    
    bool valid = !input.empty();
    std::string error;
    
    if (valid && input.length() > 8192) { valid = false; error = "input exceeds 8KB limit"; }
    if (valid && input.length() < 3) { valid = false; error = "input too short (min 3 chars)"; }
    if (valid) {
        for (char c : input) {
            if (c == 0) { valid = false; error = "null byte detected"; break; }
            if (static_cast<unsigned char>(c) < 32 &&
                c != '\n' && c != '\r' && c != '\t')
                { valid = false; error = "control character"; break; }
        }
    }
    
    std::ostringstream o;
    o << R"({"fn":")" << "verifyUserSignature" << R"(","valid":)" << (valid ? "true" : "false");
    if (!error.empty()) o << R"(,"error":")" << error << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << R"(,"alnum_count":)" << std::count_if(input.begin(), input.end(),
        [](unsigned char c) { return std::isalnum(c); });
    o << "}";
    return o.str();
}

std::string crossSignCheck(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"crossSignCheck"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "crossSignCheck" << R"(","ok":true)";
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

std::string getMasterKey(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getMasterKey" << R"(","ok":true)";
    
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
