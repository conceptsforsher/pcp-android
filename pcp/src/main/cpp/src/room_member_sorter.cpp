#include "pcp/room_member_sorter.hpp"
#include <sstream>

std::string sortByName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByPowerLevel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByPowerLevel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByJoinDate(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByJoinDate"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string filterOnline(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"filterOnline"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
