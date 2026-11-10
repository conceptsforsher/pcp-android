#include "pcp/section_sort_utils.hpp"
#include <sstream>

std::string sortByName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByTimestamp(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByTimestamp"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByPriority(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByPriority"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByActivity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByActivity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
