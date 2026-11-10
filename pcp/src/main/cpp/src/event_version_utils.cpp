#include "pcp/event_version_utils.hpp"
#include <sstream>

std::string getLatestEventVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLatestEventVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
