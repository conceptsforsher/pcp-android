#include "pcp/room_settings_utils.hpp"
#include <sstream>

std::string buildRoomSettingsUpdate(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomSettingsUpdate"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRoomSettings(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRoomSettings"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getJoinRuleLabel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getJoinRuleLabel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getHistoryVisibilityLabel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHistoryVisibilityLabel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
