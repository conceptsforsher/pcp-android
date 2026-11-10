#include "pcp/room_directory_utils.hpp"
#include <sstream>

std::string buildDirectoryFilter(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDirectoryFilter"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDirectoryEntry(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDirectoryEntry"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
