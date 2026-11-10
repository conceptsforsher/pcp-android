#include "pcp/event_relations_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildReactionRelation(const std::string& eventId, const std::string& emoji) {
    if (eventId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildReactionRelation" << R"(","input_len":)" << eventId.size();
    size_t al=0, dg=0;
    for(char c : eventId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=eventId.find('{');
    if(b!=std::string::npos){
        auto e=eventId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << eventId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildEditRelation(const std::string& eventId) {
    if (eventId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildEditRelation" << R"(","input_len":)" << eventId.size();
    size_t al=0, dg=0;
    for(char c : eventId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=eventId.find('{');
    if(b!=std::string::npos){
        auto e=eventId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << eventId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildThreadRelation(const std::string& eventId, bool fallingBack ) {
    if (eventId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildThreadRelation" << R"(","input_len":)" << eventId.size();
    size_t al=0, dg=0;
    for(char c : eventId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=eventId.find('{');
    if(b!=std::string::npos){
        auto e=eventId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << eventId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
