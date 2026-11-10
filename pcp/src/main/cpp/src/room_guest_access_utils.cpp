#include "pcp/room_guest_access_utils.hpp"
#include <sstream>

std::string parseGuestAccess(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseGuestAccess"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isGuestAllowed(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isGuestAllowed"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildGuestAccessEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildGuestAccessEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
