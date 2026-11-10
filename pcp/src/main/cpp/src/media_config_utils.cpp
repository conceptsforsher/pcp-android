#include "pcp/media_config_utils.hpp"
#include <sstream>

std::string getMediaUploadUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMediaUploadUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
