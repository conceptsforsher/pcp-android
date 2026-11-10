#include "pcp/event_age_utils.hpp"
#include <sstream>

std::string parseEventAge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseEventAge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string calculateAge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"calculateAge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isExpiredEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isExpiredEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
