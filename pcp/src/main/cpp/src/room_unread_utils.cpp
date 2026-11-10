#include "pcp/room_unread_utils.hpp"
#include <sstream>

std::string buildUnreadMarker(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUnreadMarker"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatUnreadBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatUnreadBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
