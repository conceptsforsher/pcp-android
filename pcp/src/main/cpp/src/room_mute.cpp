#include "pcp/room_mute.hpp"
#include <sstream>

std::string muteRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"muteRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string unmuteRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"unmuteRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isMuted(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isMuted"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMuteUntil(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMuteUntil"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
