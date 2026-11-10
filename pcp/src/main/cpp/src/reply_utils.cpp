#include "pcp/reply_utils.hpp"
#include <sstream>

std::string buildReplyRelation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReplyRelation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReplyBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReplyBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReplyHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReplyHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractReplyEventId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractReplyEventId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
