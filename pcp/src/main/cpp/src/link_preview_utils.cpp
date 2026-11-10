#include "pcp/link_preview_utils.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatLinkPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatLinkPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildUrlPreviewContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUrlPreviewContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDomainFromUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDomainFromUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
