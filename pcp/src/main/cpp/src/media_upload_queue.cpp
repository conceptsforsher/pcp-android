#include "pcp/media_upload_queue.hpp"
#include <sstream>

std::string enqueueUpload(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"enqueueUpload"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string dequeueUpload(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dequeueUpload"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getQueueSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getQueueSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelAllUploads(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelAllUploads"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
