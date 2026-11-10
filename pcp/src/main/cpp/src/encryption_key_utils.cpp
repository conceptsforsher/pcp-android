#include "pcp/encryption_key_utils.hpp"
#include <sstream>

std::string formatDeviceKeyFingerprint(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeviceKeyFingerprint"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRecoveryKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRecoveryKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseKeyBackupVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKeyBackupVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
