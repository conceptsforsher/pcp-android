#include "pcp/rate_limit_utils.hpp"
#include <sstream>

std::string parseRateLimitInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseRateLimitInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string calculateBackoff(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"calculateBackoff"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isRateLimited(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isRateLimited"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string resetRateLimit(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"resetRateLimit"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
