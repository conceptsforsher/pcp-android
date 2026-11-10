#include "pcp/mention_utils.hpp"
#include <sstream>

std::string extractMentions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractMentions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildMentionHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMentionHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
