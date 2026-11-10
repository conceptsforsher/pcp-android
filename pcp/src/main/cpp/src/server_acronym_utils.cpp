#include "pcp/server_acronym_utils.hpp"
#include <sstream>

std::string getServerAcronym(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getServerAcronym"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatServerName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatServerName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
