#include "pcp/room_version_utils.hpp"
#include <sstream>

std::string getLatestStableVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLatestStableVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRecommendedVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRecommendedVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
