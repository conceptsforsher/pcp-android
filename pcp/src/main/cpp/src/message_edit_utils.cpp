#include "pcp/message_edit_utils.hpp"
#include <sstream>

std::string buildEditRelation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEditRelation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildEditContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEditContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildEditFallback(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEditFallback"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getEditOriginalEventId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getEditOriginalEventId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
