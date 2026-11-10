#include "pcp/room_upload_progress.hpp"
#include <sstream>

std::string buildUploadProgressJson(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUploadProgressJson"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
