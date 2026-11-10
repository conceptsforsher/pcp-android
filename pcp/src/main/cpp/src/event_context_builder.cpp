#include "pcp/event_context_builder.hpp"
#include <sstream>

std::string buildEventContext(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildEventContext"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getParentEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getParentEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getChildEvents(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getChildEvents"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isThreadRoot(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isThreadRoot"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
