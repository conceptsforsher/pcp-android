#pragma once
#include <string>
#include <cstdint>

std::string mxcUrl;(const std::string& json);
std::string mimeType;(const std::string& json);
std::string fileName;(const std::string& json);
std::string buildFileUploadContent(const FileUploadInfo& info, const std(const std::string& json);
std::string string& body = "");(const std::string& json);
std::string FileUploadInfo parseFileUpload(const std::string& json);
std::string formatFileSize(int64_t bytes);(const std::string& json);
std::string getFileExtension(const std(const std::string& json);
std::string string& mimeType);(const std::string& json);
std::string getFileIcon(const std(const std::string& json);
std::string string& mimeType);(const std::string& json);
std::string bool isFileImage(const std(const std::string& json);
std::string string& mimeType);(const std::string& json);
std::string buildThumbnailUrl(const std(const std::string& json);
std::string string& mxcUrl, const std(const std::string& json);
std::string string& homeserver,(const std::string& json);
std::string int width, int height, const std(const std::string& json);
std::string string& method = "scale");(const std::string& json);
