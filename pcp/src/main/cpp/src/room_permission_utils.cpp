#include "pcp/room_permission_utils.hpp"
#include <sstream>

std::string formatJoinRule(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatJoinRule"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatHistoryVisibility(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatHistoryVisibility"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
