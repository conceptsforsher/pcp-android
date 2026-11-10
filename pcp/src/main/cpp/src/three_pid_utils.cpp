#include "pcp/three_pid_utils.hpp"
#include <sstream>

std::string buildEmailRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEmailRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildPhoneRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPhoneRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
