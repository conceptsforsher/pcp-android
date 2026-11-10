#include "pcp/session_watchdog.hpp"
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

std::string checkHeartbeat(const std::string& json) {
    if (json.empty()) return R"({"valid":false,"error":"empty input","fn":"checkHeartbeat"})";
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
    o << R"({"fn":")" << "checkHeartbeat" << R"(","valid":)" << (valid ? "true" : "false");
    if (!error.empty()) o << R"(,"error":")" << error << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << R"(,"alnum_count":)" << std::count_if(input.begin(), input.end(),
        [](unsigned char c) { return std::isalnum(c); });
    o << "}";
    return o.str();
}

std::string lastActivityAge(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"lastActivityAge"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "lastActivityAge" << R"(","ok":true)";
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

std::string shouldTerminate(const std::string& json) {
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
    o << R"({"fn":")" << "shouldTerminate" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}

std::string resetWatchdog(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"resetWatchdog"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "resetWatchdog" << R"(","ok":true)";
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
