#include "pcp/wellknown_helper.hpp"
#include <sstream>

std::string buildWellKnownUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildWellKnownUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractServerFromMxid(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractServerFromMxid"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildLoginUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildLoginUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
