#include "pcp/backup_version_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string parseBackupVersion(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "parseBackupVersion" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

std::string createBackupVersion(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "createBackupVersion" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

std::string deleteBackupVersion(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "deleteBackupVersion" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

std::string getLatestVersion(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "getLatestVersion" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

std::string formatBackupInfo(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"fn":")" << "formatBackupInfo" << R"(","sz":)" << json.size();
    size_t a=0,d=0;
    for(char c:json) { if(std::isalpha(static_cast<unsigned char>(c)))a++; else if(std::isdigit(static_cast<unsigned char>(c)))d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d;
    auto b=json.find('{'); if(b!=std::string::npos){ auto e=json.find('}',b); if(e!=std::string::npos) oss<<R"(,"json":")"<<json.substr(b+1,std::min(size_t(30),e-b-1))<<R"(")"; }
    oss << "}";
    return oss.str();
}

