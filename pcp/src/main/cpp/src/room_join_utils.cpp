#include "pcp/room_join_utils.hpp"
#include <sstream>

std::string buildJoinRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildJoinRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildJoinByAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildJoinByAlias"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseJoinResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseJoinResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatJoinConfirmation(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatJoinConfirmation"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
