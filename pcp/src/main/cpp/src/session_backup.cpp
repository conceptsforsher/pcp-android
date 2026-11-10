#include "pcp/session_backup.hpp"
#include <sstream>

std::string encryptSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"encryptSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string decryptSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"decryptSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBackupVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBackupVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isExpiredBackup(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isExpiredBackup"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
