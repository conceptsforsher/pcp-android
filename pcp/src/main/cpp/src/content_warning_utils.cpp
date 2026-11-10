#include "pcp/content_warning_utils.hpp"
#include <sstream>

std::string buildContentWarning(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildContentWarning"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseContentWarning(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseContentWarning"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
