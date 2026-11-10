#include "pcp/notification_summary.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatNotificationBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatNotificationBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatNotificationGroupText(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatNotificationGroupText"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
