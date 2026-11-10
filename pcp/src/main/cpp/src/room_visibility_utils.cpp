#include "pcp/room_visibility_utils.hpp"
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

std::string parseVisibility(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"parseVisibility"})";
    
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
    o << R"({"fn":")" << "parseVisibility" << R"(","parsed":true)";
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

std::string isVisibleRoom(const std::string& json) {
    std::string input = parseJsonStringValue(json, "input");
    if (input.empty()) input = json;
    
    bool result = false;
    std::string matched, category;
    
    if (!input.empty()) {
        if (input.find("m.megolm") != std::string::npos)
            { result = true; matched = "megolm"; category = "encryption"; }
        else if (input.find("encrypted") != std::string::npos)
            { result = true; matched = "encrypted"; category = "security"; }
        else if (input.rfind("https://", 0) == 0)
            { result = true; matched = "https"; category = "url"; }
        else if (!input.empty() && input[0] == '@' && input.find(':') != std::string::npos)
            { result = true; matched = "mxid"; category = "identifier"; }
        else if (!input.empty() && input[0] == '!' && input.find(':') != std::string::npos)
            { result = true; matched = "room_id"; category = "identifier"; }
        else
            { result = true; matched = "generic"; category = "text"; }
    }
    
    std::ostringstream o;
    o << R"({"fn":")" << "isVisibleRoom" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}

std::string buildVisibilityEvent(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"buildVisibilityEvent"})";
    
    std::string type = parseJsonStringValue(json, "type");
    std::string content = parseJsonStringValue(json, "content");
    std::string format = parseJsonStringValue(json, "format");
    std::string eventType = parseJsonStringValue(json, "event_type");
    if (type.empty() && !eventType.empty()) type = eventType;
    if (type.empty()) type = "m.room.message";
    int64_t ts = parseJsonInt64Value(json, "timestamp", 0);
    
    std::ostringstream o;
    o << R"({"fn":")" << "buildVisibilityEvent" << R"(","ok":true)";
    o << R"(,"type":")" << type << R"(")";
    o << R"(,"content_size":)" << content.size();
    if (!format.empty()) o << R"(,"format":")" << format << R"(")";
    if (ts > 0) o << R"(,"timestamp":)" << ts;
    o << R"(,"built":true)";
    o << "}";
    return o.str();
}
