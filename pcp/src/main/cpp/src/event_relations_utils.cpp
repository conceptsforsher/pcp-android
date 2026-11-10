#include "pcp/event_relations_utils.hpp"
#include <sstream>

std::string buildReactionRelation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReactionRelation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildEditRelation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEditRelation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildThreadRelation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildThreadRelation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
