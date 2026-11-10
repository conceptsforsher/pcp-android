#include "pcp/room_invite_utils.hpp"
#include <sstream>

std::string buildInviteRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildInviteRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string build3pidInviteRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"build3pidInviteRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatInviteNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatInviteNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
