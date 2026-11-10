#include "pcp/sas_verification_utils.hpp"
#include <sstream>

std::string formatSasEmojis(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSasEmojis"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSasDecimals(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSasDecimals"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractSasBytes(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractSasBytes"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
