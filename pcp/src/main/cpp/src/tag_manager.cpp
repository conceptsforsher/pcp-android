#include "pcp/tag_manager.hpp"
#include <sstream>

std::string getRoomTags(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomTags"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string setRoomTag(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"setRoomTag"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string removeRoomTag(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"removeRoomTag"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isFavorited(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isFavorited"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
