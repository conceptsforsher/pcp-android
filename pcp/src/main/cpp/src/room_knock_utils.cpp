#include "pcp/room_knock_utils.hpp"
#include <sstream>

std::string buildKnockRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildKnockRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatKnockResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatKnockResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
