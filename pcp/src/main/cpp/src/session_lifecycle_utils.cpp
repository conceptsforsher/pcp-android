#include "pcp/session_lifecycle_utils.hpp"
#include <sstream>

std::string buildLogoutRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildLogoutRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildLogoutAllRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildLogoutAllRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseSessionState(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSessionState"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
