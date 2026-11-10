#include "pcp/user_directory_search.hpp"
#include <sstream>

std::string searchUsers(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"searchUsers"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseSearchResults(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSearchResults"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isExactMatch(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isExactMatch"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string rankByRelevance(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"rankByRelevance"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
