#include "pcp/encryption_status.hpp"
#include <sstream>

std::string getRoomEncryption(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomEncryption"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isEncrypted(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isEncrypted"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getAlgorithm(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getAlgorithm"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isKeyShared(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isKeyShared"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
