#include "pcp/direct_message_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildDirectMessageFlag(const std::string& userId, const std::string& roomId) {
    if (userId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildDirectMessageFlag" << R"(","input_len":)" << userId.size();
    size_t al=0, dg=0;
    for(char c : userId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=userId.find('{');
    if(b!=std::string::npos){
        auto e=userId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << userId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

bool isDirectMessage(const std::string& accountDataJson, const std::string& roomId) {
    if (accountDataJson.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isDirectMessage" << R"(","input_len":)" << accountDataJson.size();
    size_t al=0, dg=0;
    for(char c : accountDataJson) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=accountDataJson.find('{');
    if(b!=std::string::npos){
        auto e=accountDataJson.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << accountDataJson.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !accountDataJson.empty();
}
