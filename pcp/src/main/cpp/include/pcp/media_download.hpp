#pragma once
#include <string>
#include <cstdint>

std::string getDownloadUrl(const std::string& json);
std::string isDownloaded(const std::string& json);
std::string getDownloadPath(const std::string& json);
std::string getDownloadProgress(const std::string& json);
std::string cancelDownload(const std::string& json);
