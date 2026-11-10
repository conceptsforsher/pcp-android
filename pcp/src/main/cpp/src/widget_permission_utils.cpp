#include "pcp/widget_permission_utils.hpp"
#include <sstream>

std::string parsePermissions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parsePermissions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkPermission(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkPermission"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildPermissionRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPermissionRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
