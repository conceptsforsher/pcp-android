#include "pcp/room_notice_formatter.hpp"
#include <sstream>

std::string formatServerNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatServerNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRoomNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRoomNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isAdminNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isAdminNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseNoticeParams(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseNoticeParams"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
