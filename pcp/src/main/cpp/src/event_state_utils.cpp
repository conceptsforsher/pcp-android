#include "pcp/event_state_utils.hpp"
#include <sstream>

std::string parseStateKey(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseStateKey"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildStateEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildStateEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatStateEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatStateEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
