#include "pcp/content_builder_utils.hpp"
#include <sstream>

std::string buildTextContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildTextContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildNoticeContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildNoticeContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildEmoteContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEmoteContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildImageContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildImageContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
