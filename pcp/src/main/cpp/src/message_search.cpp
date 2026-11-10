#include "pcp/message_search.hpp"
#include <sstream>

std::string buildSearchRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSearchRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractSearchContext(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractSearchContext"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
