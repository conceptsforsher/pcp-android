#include "pcp/user_status_message_utils.hpp"
#include <sstream>

std::string buildStatusMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildStatusMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseStatusMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseStatusMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
