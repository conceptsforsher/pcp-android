#include "pcp/message_format_helper.hpp"
#include <sstream>

std::string formatMessageBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMessageBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatNotificationBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatNotificationBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatMessageSender(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMessageSender"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildMentionPill(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMentionPill"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
