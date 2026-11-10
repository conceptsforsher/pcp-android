#include "pcp/read_marker_utils.hpp"
#include <sstream>

std::string buildFullyReadMarker(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildFullyReadMarker"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReadReceipt(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReadReceipt"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseReadMarkerEventId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseReadMarkerEventId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReadMarkerAccessibility(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReadMarkerAccessibility"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
