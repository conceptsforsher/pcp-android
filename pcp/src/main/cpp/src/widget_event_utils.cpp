#include "pcp/widget_event_utils.hpp"
#include <sstream>

std::string buildWidgetApiRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildWidgetApiRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatWidgetBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatWidgetBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getWidgetIcon(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getWidgetIcon"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
