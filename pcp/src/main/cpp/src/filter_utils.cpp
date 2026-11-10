#include "pcp/filter_utils.hpp"
#include <sstream>

std::string parseFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildFilterQuery(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildFilterQuery"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string matchesFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"matchesFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
