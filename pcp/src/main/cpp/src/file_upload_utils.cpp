#include "pcp/file_upload_utils.hpp"
#include <sstream>

std::string buildFileUploadContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildFileUploadContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatFileSize(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatFileSize"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getFileExtension(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFileExtension"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getFileIcon(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFileIcon"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
