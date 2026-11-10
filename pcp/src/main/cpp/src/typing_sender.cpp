#include "pcp/typing_sender.hpp"
#include <sstream>

std::string sendTyping(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sendTyping"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string stopTyping(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"stopTyping"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getTypingTimeout(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getTypingTimeout"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isCurrentlyTyping(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isCurrentlyTyping"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
