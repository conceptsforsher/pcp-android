#include "pcp/event_aggregation_utils.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildMergedEventHeader(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMergedEventHeader"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
