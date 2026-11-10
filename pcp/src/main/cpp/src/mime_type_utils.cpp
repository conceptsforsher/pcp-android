#include "pcp/mime_type_utils.hpp"
#include <sstream>

std::string parseMimeType(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseMimeType"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSupportedMimeType(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSupportedMimeType"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getExtensionFromMime(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getExtensionFromMime"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMimeFromExtension(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMimeFromExtension"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
