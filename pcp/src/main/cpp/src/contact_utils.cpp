#include "pcp/contact_utils.hpp"
#include <sstream>

std::string normalizePhoneNumber(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"normalizePhoneNumber"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatPhoneForDisplay(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPhoneForDisplay"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
