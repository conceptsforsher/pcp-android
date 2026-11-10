#include "pcp/event_display_formatter.hpp"
#include <sstream>

std::string formatSenderDisplay(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatSenderDisplay"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatEventBubbleTime(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatEventBubbleTime"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatEditedNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatEditedNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReplyPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReplyPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
