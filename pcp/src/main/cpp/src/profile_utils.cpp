#include "pcp/profile_utils.hpp"
#include <sstream>

std::string buildProfileRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildProfileRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildProfileUpdate(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildProfileUpdate"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildDisplayNameChange(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDisplayNameChange"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildAvatarUrlChange(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildAvatarUrlChange"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
