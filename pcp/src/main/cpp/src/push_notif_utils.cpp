#include "pcp/push_notif_utils.hpp"
#include <sstream>

std::string buildNotificationGroupKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildNotificationGroupKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildNotificationSummary(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildNotificationSummary"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
