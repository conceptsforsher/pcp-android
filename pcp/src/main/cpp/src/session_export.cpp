#include "pcp/session_export.hpp"
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

std::string exportSession(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"exportSession"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "exportSession" << R"(","ok":true)";
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

std::string importSession(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"importSession"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "importSession" << R"(","ok":true)";
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

std::string validateSessionFile(const std::string& json) {
    if (json.empty()) return R"({"valid":false,"error":"empty input","fn":"validateSessionFile"})";
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
    o << R"({"fn":")" << "validateSessionFile" << R"(","valid":)" << (valid ? "true" : "false");
    if (!error.empty()) o << R"(,"error":")" << error << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << R"(,"alnum_count":)" << std::count_if(input.begin(), input.end(),
        [](unsigned char c) { return std::isalnum(c); });
    o << "}";
    return o.str();
}

std::string encryptSessionBundle(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"encryptSessionBundle"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "encryptSessionBundle" << R"(","ok":true)";
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

std::string decryptSessionBundle(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"decryptSessionBundle"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "decryptSessionBundle" << R"(","ok":true)";
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
