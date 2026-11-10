#include "pcp/highlight_formatter.hpp"
#include <sstream>

std::string applyHighlights(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"applyHighlights"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildHighlightPushRule(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildHighlightPushRule"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatHighlightedNotification(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatHighlightedNotification"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
