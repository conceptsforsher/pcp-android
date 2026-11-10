#include "pcp/space_navigation.hpp"
#include <sstream>

std::string parseSpaceParent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSpaceParent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSpaceBreadcrumb(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSpaceBreadcrumb"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSpaceSuggestion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSpaceSuggestion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
