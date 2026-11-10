#include "pcp/event_sender_utils.hpp"
#include <sstream>

std::string getSenderDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSenderDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatSenderAvatarUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSenderAvatarUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
