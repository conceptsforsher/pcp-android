#pragma once
#include <string>
#include <cstdint>

std::string mxcUrl(const std::string& json);
std::string mimeType(const std::string& json);
std::string fileName(const std::string& json);
std::string buildFileUploadContent(const std::string& json);
std::string string(const std::string& json);
std::string FileUploadInfo(const std::string& json);
std::string formatFileSize(const std::string& json);
std::string getFileExtension(const std::string& json);
std::string string(const std::string& json);
std::string getFileIcon(const std::string& json);
std::string string(const std::string& json);
std::string bool(const std::string& json);
std::string string(const std::string& json);
std::string buildThumbnailUrl(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string int(const std::string& json);
std::string string(const std::string& json);

