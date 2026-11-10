#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string scanMedia(const std::string& json);
std::string isSafeContent(const std::string& json);
std::string getMediaDimensions(const std::string& json);
std::string detectBlurHash(const std::string& json);
std::string formatMediaPreview(const std::string& json);
