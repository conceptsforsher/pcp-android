#pragma once
#include <string>
#include <cstdint>

int64_t getMaxUploadSize(const std::string& serverConfigJson);
std::string getMediaUploadUrl(const std::string& homeserver);
bool isMimeTypeSupported(const std::string& mimeType, const std::string& serverConfigJson);
