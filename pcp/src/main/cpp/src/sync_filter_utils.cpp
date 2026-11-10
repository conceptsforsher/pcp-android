#include "pcp/sync_filter_utils.hpp"
#include <sstream>

std::string parseFilterQuery(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseFilterQuery"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSyncFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSyncFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string mergeFilterSets(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"mergeFilterSets"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDefaultFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
