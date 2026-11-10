#include "pcp/megolm_utils.hpp"
#include <sstream>

std::string createSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"createSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string encryptGroupMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"encryptGroupMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string decryptGroupMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"decryptGroupMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSessionKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSessionKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
