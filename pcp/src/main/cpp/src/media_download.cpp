#include "pcp/media_download.hpp"
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

std::string getDownloadUrl(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getDownloadUrl" << R"(","ok":true)";
    
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

std::string isDownloaded(const std::string& json) {
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
    o << R"({"fn":")" << "isDownloaded" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}

std::string getDownloadPath(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getDownloadPath" << R"(","ok":true)";
    
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

std::string getDownloadProgress(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":")" << "getDownloadProgress" << R"(","ok":true)";
    
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

std::string cancelDownload(const std::string& json) {
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
    o << R"({"fn":")" << "cancelDownload" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}
