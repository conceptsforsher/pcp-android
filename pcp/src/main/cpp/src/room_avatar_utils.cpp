#include "pcp/room_avatar_utils.hpp"
#include <sstream>

std::string generateRoomInitials(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"generateRoomInitials"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRoomColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRoomAvatarUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRoomAvatarUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoomAvatarChange(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomAvatarChange"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
