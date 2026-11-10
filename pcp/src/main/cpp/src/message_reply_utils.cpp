#include "pcp/message_reply_utils.hpp"
#include <sstream>

std::string buildReplyFormattedBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReplyFormattedBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReplyPlainBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReplyPlainBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractRepliedEventId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractRepliedEventId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
