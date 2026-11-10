#include "pcp/room_member_utils.hpp"
#include <sstream>

std::string formatMemberCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMemberCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMemberCountWithOnline(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMemberCountWithOnline"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMembershipEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMembershipEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string membershipToString(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"membershipToString"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
