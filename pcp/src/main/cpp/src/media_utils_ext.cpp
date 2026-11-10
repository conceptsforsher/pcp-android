#include "pcp/media_utils_ext.hpp"
#include <sstream>

std::string buildDownloadUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDownloadUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildThumbnailUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildThumbnailUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractMxcServer(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractMxcServer"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractMxcMediaId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractMxcMediaId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
