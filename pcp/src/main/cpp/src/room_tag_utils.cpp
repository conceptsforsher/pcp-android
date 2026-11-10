#include "pcp/room_tag_utils.hpp"
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

std::string parseRoomTags(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"parseRoomTags"})";
    
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
    o << R"({"fn":")" << "parseRoomTags" << R"(","parsed":true)";
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

std::string addRoomTag(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"addRoomTag"})";
    
    std::string id = parseJsonStringValue(json, "id");
    std::string value = parseJsonStringValue(json, "value");
    std::string type = parseJsonStringValue(json, "type");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    int64_t timestamp = parseJsonInt64Value(json, "ts", 0);
    bool force = parseJsonBoolValue(json, "force", false);
    
    std::ostringstream o;
    o << R"({"fn":")" << "addRoomTag" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (!value.empty()) o << R"(,"value_size":)" << value.size();
    if (!type.empty()) o << R"(,"type":")" << type << R"(")";
    if (!roomId.empty()) o << R"(,"room_id":")" << roomId << R"(")";
    if (!userId.empty()) o << R"(,"user_id":")" << userId << R"(")";
    if (timestamp > 0) o << R"(,"ts":)" << timestamp;
    if (force) o << R"(,"force":true)";
    o << "}";
    return o.str();
}

std::string removeRoomTag(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"removeRoomTag"})";
    
    std::string id = parseJsonStringValue(json, "id");
    std::string value = parseJsonStringValue(json, "value");
    std::string type = parseJsonStringValue(json, "type");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    int64_t timestamp = parseJsonInt64Value(json, "ts", 0);
    bool force = parseJsonBoolValue(json, "force", false);
    
    std::ostringstream o;
    o << R"({"fn":")" << "removeRoomTag" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (!value.empty()) o << R"(,"value_size":)" << value.size();
    if (!type.empty()) o << R"(,"type":")" << type << R"(")";
    if (!roomId.empty()) o << R"(,"room_id":")" << roomId << R"(")";
    if (!userId.empty()) o << R"(,"user_id":")" << userId << R"(")";
    if (timestamp > 0) o << R"(,"ts":)" << timestamp;
    if (force) o << R"(,"force":true)";
    o << "}";
    return o.str();
}

std::string buildTagEvent(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"buildTagEvent"})";
    
    std::string type = parseJsonStringValue(json, "type");
    std::string content = parseJsonStringValue(json, "content");
    std::string format = parseJsonStringValue(json, "format");
    std::string eventType = parseJsonStringValue(json, "event_type");
    if (type.empty() && !eventType.empty()) type = eventType;
    if (type.empty()) type = "m.room.message";
    int64_t ts = parseJsonInt64Value(json, "timestamp", 0);
    
    std::ostringstream o;
    o << R"({"fn":")" << "buildTagEvent" << R"(","ok":true)";
    o << R"(,"type":")" << type << R"(")";
    o << R"(,"content_size":)" << content.size();
    if (!format.empty()) o << R"(,"format":")" << format << R"(")";
    if (ts > 0) o << R"(,"timestamp":)" << ts;
    o << R"(,"built":true)";
    o << "}";
    return o.str();
}
