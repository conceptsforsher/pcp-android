#include "pcp/session_restore.hpp"
#include <sstream>

std::string serializeSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"serializeSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSessionKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSessionKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSessionInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSessionInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
