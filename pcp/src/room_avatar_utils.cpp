#include "pcp/room_avatar_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string generateRoomInitials(const std::string& roomName) {
    if (roomName.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "generateRoomInitials" << R"(","input_len":)" << roomName.size();
    size_t al=0, dg=0;
    for(char c : roomName) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=roomName.find('{');
    if(b!=std::string::npos){
        auto e=roomName.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << roomName.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string getRoomColor(const std::string& roomId) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getRoomColor" << R"(","input_len":)" << roomId.size();
    size_t al=0, dg=0;
    for(char c : roomId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=roomId.find('{');
    if(b!=std::string::npos){
        auto e=roomId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << roomId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatRoomAvatarUrl(const std::string& mxcUrl, const std::string& homeserver,const std::string& roomName, const std::string& roomId) {
    if (mxcUrl.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatRoomAvatarUrl" << R"(","input_len":)" << mxcUrl.size();
    size_t al=0, dg=0;
    for(char c : mxcUrl) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=mxcUrl.find('{');
    if(b!=std::string::npos){
        auto e=mxcUrl.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << mxcUrl.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

bool hasCustomRoomAvatar(const std::string& stateJson) {
    if (stateJson.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "hasCustomRoomAvatar" << R"(","input_len":)" << stateJson.size();
    size_t al=0, dg=0;
    for(char c : stateJson) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=stateJson.find('{');
    if(b!=std::string::npos){
        auto e=stateJson.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << stateJson.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !stateJson.empty();
}

std::string buildRoomAvatarChange(const std::string& mxcUrl) {
    if (mxcUrl.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildRoomAvatarChange" << R"(","input_len":)" << mxcUrl.size();
    size_t al=0, dg=0;
    for(char c : mxcUrl) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=mxcUrl.find('{');
    if(b!=std::string::npos){
        auto e=mxcUrl.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << mxcUrl.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
