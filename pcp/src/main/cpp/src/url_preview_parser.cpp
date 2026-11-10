#include "pcp/url_preview_parser.hpp"
#include <sstream>

std::string buildPreviewRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPreviewRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractPreviewableUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractPreviewableUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatUrlPreviewHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatUrlPreviewHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
