#include "pcp/member_list.hpp"
#include <sstream>

std::string buildMemberContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMemberContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMemberDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMemberDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string memberListToJson(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"memberListToJson"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string memberToJson(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"memberToJson"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
