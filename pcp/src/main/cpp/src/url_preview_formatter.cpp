#include "pcp/url_preview_formatter.hpp"
#include <sstream>

std::string extractFirstUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractFirstUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatUrlPreviewCard(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatUrlPreviewCard"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
