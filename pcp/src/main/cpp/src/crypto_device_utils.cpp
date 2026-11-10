#include "pcp/crypto_device_utils.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDeviceKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeviceKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDeviceInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeviceInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
