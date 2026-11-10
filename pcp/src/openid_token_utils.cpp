#include "pcp/openid_token_utils.hpp"
#include <sstream>
#include <algorithm>

std::string parseOpenIdToken(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"parseOpenIdToken"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string buildOpenIdRequest(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"buildOpenIdRequest"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string validateToken(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"validateToken"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string getMatrixId(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getMatrixId"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string formatTokenInfo(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatTokenInfo"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

