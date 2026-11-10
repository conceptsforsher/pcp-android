#include "pcp/server_version_check.hpp"
#include <sstream>

std::string parseServerVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseServerVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isVersionSupported(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isVersionSupported"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string compareVersions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"compareVersions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMinRequiredVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMinRequiredVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
