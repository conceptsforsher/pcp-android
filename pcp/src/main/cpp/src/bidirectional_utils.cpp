#include "pcp/bidirectional_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string filterBidiOverrides(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "filterBidiOverrides" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

std::string sanitizeDisplayText(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "sanitizeDisplayText" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

