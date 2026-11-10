#include "pcp/room_history_utils.hpp"
#include <sstream>

std::string getDefaultHistoryVisibility(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultHistoryVisibility"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatHistoryVisibility(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatHistoryVisibility"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
