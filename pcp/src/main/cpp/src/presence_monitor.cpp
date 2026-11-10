#include "pcp/presence_monitor.hpp"
#include <sstream>

std::string toJson(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"toJson"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
