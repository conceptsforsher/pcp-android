#include "pcp/media_progress_utils.hpp"
#include <sstream>

std::string buildMediaProgressJson(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMediaProgressJson"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMediaProgress(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMediaProgress"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
