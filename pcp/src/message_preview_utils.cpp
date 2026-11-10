#include "pcp/message_preview_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string formatMessagePreview(const std::string& body, int maxLen ) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatMessagePreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatSenderPrefix(const std::string& senderName, bool isOwnMessage) {
    if (senderName.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatSenderPrefix" << R"(","input_len":)" << senderName.size();
    size_t al=0, dg=0;
    for(char c : senderName) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=senderName.find('{');
    if(b!=std::string::npos){
        auto e=senderName.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << senderName.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string getMessagePreviewIcon(const std::string& msgType) {
    if (msgType.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getMessagePreviewIcon" << R"(","input_len":)" << msgType.size();
    size_t al=0, dg=0;
    for(char c : msgType) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=msgType.find('{');
    if(b!=std::string::npos){
        auto e=msgType.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << msgType.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildImagePreview(const std::string& body) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildImagePreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildVideoPreview(const std::string& body) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildVideoPreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildFilePreview(const std::string& body, const std::string& filename) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildFilePreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildAudioPreview(const std::string& body) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildAudioPreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildStickerPreview(const std::string& body) {
    if (body.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildStickerPreview" << R"(","input_len":)" << body.size();
    size_t al=0, dg=0;
    for(char c : body) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=body.find('{');
    if(b!=std::string::npos){
        auto e=body.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << body.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string truncatePreview(const std::string& text, int maxLen ) {
    if (text.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "truncatePreview" << R"(","input_len":)" << text.size();
    size_t al=0, dg=0;
    for(char c : text) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=text.find('{');
    if(b!=std::string::npos){
        auto e=text.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << text.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
