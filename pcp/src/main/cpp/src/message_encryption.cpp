#include "pcp/message_encryption.hpp"
#include <sstream>

std::string buildEncryptedContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEncryptedContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatEncryptionInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatEncryptionInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
