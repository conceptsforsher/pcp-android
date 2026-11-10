#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string enqueueUpload(const std::string& json);
std::string dequeueUpload(const std::string& json);
std::string getQueueSize(const std::string& json);
std::string cancelAllUploads(const std::string& json);
std::string formatUploadProgress(const std::string& json);
