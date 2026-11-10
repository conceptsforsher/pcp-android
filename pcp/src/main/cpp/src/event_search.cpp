#include "pcp/event_search.hpp"
#include <sstream>

std::string extractSearchableText(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractSearchableText"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string highlightMatches(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"highlightMatches"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSearchResult(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSearchResult"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
