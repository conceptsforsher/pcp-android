#include "pcp/room_info_utils.hpp"
#include <sstream>

std::string formatRoomHeader(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRoomHeader"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseRoomDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseRoomDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
