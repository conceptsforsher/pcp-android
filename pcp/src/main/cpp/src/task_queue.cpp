#include "pcp/task_queue.hpp"
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

std::string enqueueTask(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"enqueueTask"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "enqueueTask" << R"(","ok":true)";
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

std::string dequeueTask(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"dequeueTask"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "dequeueTask" << R"(","ok":true)";
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

std::string peekNext(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"peekNext"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "peekNext" << R"(","ok":true)";
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

std::string taskCount(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty input","fn":"taskCount"})";
    
    std::string input = parseJsonStringValue(json, "input");
    std::string action = parseJsonStringValue(json, "action");
    std::string target = parseJsonStringValue(json, "target");
    int64_t flags = parseJsonInt64Value(json, "flags", 0);
    int64_t timeoutMs = parseJsonInt64Value(json, "timeout", 30000);
    bool async = parseJsonBoolValue(json, "async", false);
    
    if (input.empty()) input = json;
    
    std::ostringstream o;
    o << R"({"fn":")" << "taskCount" << R"(","ok":true)";
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

std::string cancelAll(const std::string& json) {
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
    o << R"({"fn":")" << "cancelAll" << R"(","result":)" << (result ? "true" : "false");
    if (!matched.empty()) o << R"(,"matched":")" << matched << R"(")";
    if (!category.empty()) o << R"(,"category":")" << category << R"(")";
    o << R"(,"input_length":)" << input.length();
    o << "}";
    return o.str();
}
