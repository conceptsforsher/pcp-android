#include "pcp/direct_message_utils.hpp"
#include <sstream>

std::string buildDirectMessageFlag(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDirectMessageFlag"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
