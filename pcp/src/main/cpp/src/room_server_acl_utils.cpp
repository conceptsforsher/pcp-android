#include "pcp/room_server_acl_utils.hpp"
#include <sstream>

std::string buildAclEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildAclEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
