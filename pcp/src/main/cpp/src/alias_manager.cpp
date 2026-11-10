#include "pcp/alias_manager.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string parseAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseAlias"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string addRoomAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"addRoomAlias"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string removeRoomAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"removeRoomAlias"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string getLocalAliases(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLocalAliases"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string resolveRoomAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"resolveRoomAlias"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
