#include "pcp/invite_counter.hpp"
#include <sstream>

std::string countPendingInvites(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"countPendingInvites"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getOldestInvite(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getOldestInvite"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string hasInviteFrom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"hasInviteFrom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatInviteSummary(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatInviteSummary"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
