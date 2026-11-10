#include "pcp/room_retention_utils.hpp"
#include <sstream>

std::string buildRetentionEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRetentionEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
