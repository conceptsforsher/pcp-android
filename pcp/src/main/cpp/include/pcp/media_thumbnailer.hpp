#pragma once
#include <string>
#include <cstdint>

std::string generateThumbnail(const std::string& json);
std::string getThumbnailSize(const std::string& json);
std::string isAnimated(const std::string& json);
std::string getMimeForThumb(const std::string& json);
std::string cacheThumbnailKey(const std::string& json);
