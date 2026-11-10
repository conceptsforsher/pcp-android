#include "pcp/sync_summary_utils.hpp"
#include <sstream>

std::string formatSyncSummary(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSyncSummary"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
