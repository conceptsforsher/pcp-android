#include "pcp/device_naming_helper.hpp"
#include <sstream>

std::string generateDeviceName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"generateDeviceName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseDeviceNameFromUserAgent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseDeviceNameFromUserAgent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDeviceDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeviceDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string truncateDeviceName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"truncateDeviceName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
