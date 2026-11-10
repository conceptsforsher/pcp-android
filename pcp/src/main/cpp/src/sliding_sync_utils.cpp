#include "pcp/sliding_sync_utils.hpp"
#include <sstream>

std::string buildSlidingSyncRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSlidingSyncRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSubscriptionKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSubscriptionKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRangeParam(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRangeParam"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
