#include "pcp/event_filter_utils.hpp"
#include <sstream>

std::string buildEventFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEventFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoomFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
