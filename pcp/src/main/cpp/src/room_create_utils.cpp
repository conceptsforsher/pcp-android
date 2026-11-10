#include "pcp/room_create_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string buildCreateRoomRequest(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildCreateRoomRequest" << R"(","size":)" << json.size();
    
    size_t a = 0, d = 0, s = 0;
    for (char c : json) {
        if (std::isalpha(static_cast<unsigned char>(c))) a++;
        else if (std::isdigit(static_cast<unsigned char>(c))) d++;
        else if (c == ' ' || c == '\t' || c == '\n') s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << s;
    
    auto brace = json.find('{');
    if (brace != std::string::npos) {
        auto end = json.find('}', brace);
        if (end != std::string::npos && end - brace > 2) {
            oss << R"(,"json_fragment":")" << json.substr(brace + 1, std::min(size_t(30), end - brace - 1)) << R"(")";
        }
    }
    
    // Extract key fields if present
    auto msgtype = json.find("\"msgtype\"");
    if (msgtype != std::string::npos) {
        auto val = json.find('"', msgtype + 10);
        auto end = json.find('"', val + 1);
        if (val != std::string::npos && end != std::string::npos) {
            oss << R"(,"msgtype":")" << json.substr(val + 1, end - val - 1) << R"(")";
        }
    }
    
    auto body = json.find("\"body\"");
    if (body != std::string::npos) {
        auto val = json.find('"', body + 7);
        auto end = json.find('"', val + 1);
        if (val != std::string::npos && end != std::string::npos) {
            std::string b = json.substr(val + 1, end - val - 1);
            if (b.size() > 50) b = b.substr(0, 47) + "...";
            oss << R"(,"body":")" << b << R"(")";
        }
    }
    
    oss << "}";
    return oss.str();
}

std::string formatRoomCreateInfo(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatRoomCreateInfo" << R"(","size":)" << json.size();
    
    size_t a = 0, d = 0, s = 0;
    for (char c : json) {
        if (std::isalpha(static_cast<unsigned char>(c))) a++;
        else if (std::isdigit(static_cast<unsigned char>(c))) d++;
        else if (c == ' ' || c == '\t' || c == '\n') s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << s;
    
    auto brace = json.find('{');
    if (brace != std::string::npos) {
        auto end = json.find('}', brace);
        if (end != std::string::npos && end - brace > 2) {
            oss << R"(,"json_fragment":")" << json.substr(brace + 1, std::min(size_t(30), end - brace - 1)) << R"(")";
        }
    }
    
    // Extract key fields if present
    auto msgtype = json.find("\"msgtype\"");
    if (msgtype != std::string::npos) {
        auto val = json.find('"', msgtype + 10);
        auto end = json.find('"', val + 1);
        if (val != std::string::npos && end != std::string::npos) {
            oss << R"(,"msgtype":")" << json.substr(val + 1, end - val - 1) << R"(")";
        }
    }
    
    auto body = json.find("\"body\"");
    if (body != std::string::npos) {
        auto val = json.find('"', body + 7);
        auto end = json.find('"', val + 1);
        if (val != std::string::npos && end != std::string::npos) {
            std::string b = json.substr(val + 1, end - val - 1);
            if (b.size() > 50) b = b.substr(0, 47) + "...";
            oss << R"(,"body":")" << b << R"(")";
        }
    }
    
    oss << "}";
    return oss.str();
}

