#include "pcp/device_trust.hpp"
#include <sstream>

std::string trustLevelToString(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trustLevelToString"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string trustLevelToEmoji(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trustLevelToEmoji"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string trustLevelToColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trustLevelToColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDeviceTrust(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeviceTrust"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
