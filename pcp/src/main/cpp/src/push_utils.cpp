#include "pcp/push_utils.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildPushRuleContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPushRuleContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDefaultPushAction(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultPushAction"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
