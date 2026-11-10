#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string cachePreview(const std::string& json);
std::string getCachedPreview(const std::string& json);
std::string isPreviewStale(const std::string& json);
std::string clearPreviewCache(const std::string& json);
std::string formatCachedPreview(const std::string& json);
