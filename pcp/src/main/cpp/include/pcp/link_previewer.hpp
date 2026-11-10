#pragma once
#include <string>
#include <cstdint>

std::string parseLinkMetadata(const std::string& json);
std::string getOgpTags(const std::string& json);
std::string isPreviewable(const std::string& json);
std::string formatPreviewHtml(const std::string& json);
std::string cachePreview(const std::string& json);
