#pragma once
#include <string>
#include <cstdint>

std::string computeThumbnailSize(const std::string& json);
std::string generateThumbnailUrl(const std::string& json);
std::string parseThumbnailInfo(const std::string& json);
std::string getBestThumbnail(const std::string& json);
