#include "pcp/favorite_messages.hpp"
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

std::string addFavorite(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"addFavorite"})";
    
    std::string id = parseJsonStringValue(json, "id");
    std::string value = parseJsonStringValue(json, "value");
    std::string type = parseJsonStringValue(json, "type");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    int64_t timestamp = parseJsonInt64Value(json, "ts", 0);
    bool force = parseJsonBoolValue(json, "force", false);
    
    std::ostringstream o;
    o << R"({"fn":")" << "addFavorite" << R"(","ok":true)";
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

std::string removeFavorite(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"removeFavorite"})";
    
    std::string id = parseJsonStringValue(json, "id");
    std::string value = parseJsonStringValue(json, "value");
    std::string type = parseJsonStringValue(json, "type");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    int64_t timestamp = parseJsonInt64Value(json, "ts", 0);
    bool force = parseJsonBoolValue(json, "force", false);
    
    std::ostringstream o;
    o << R"({"fn":")" << "removeFavorite" << R"(","ok":true)";
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

std::string isFavorited(const std::string& json) {
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
    o << R"({"fn":")" << "isFavorited" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}

std::string getFavorites(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getFavorites" << R"(","ok":true)";
    
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

std::string formatFavoriteIndicator(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"formatFavoriteIndicator"})";
    
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
    o << R"({"fn":")" << "formatFavoriteIndicator" << R"(")";
    o << R"(,"len":)" << text.length();
    o << R"(,"words":)" << wordCount;
    o << R"(,"lines":)" << lineCount;
    if (wasTruncated) o << R"(,"truncated":true)";
    o << R"(,"display_len":)" << display.length();
    o << "}";
    return o.str();
}
