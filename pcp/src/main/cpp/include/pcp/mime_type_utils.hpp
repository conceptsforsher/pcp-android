#pragma once
#include <string>
#include <cstdint>

std::string parseMimeType(const std::string& json);
std::string isSupportedMimeType(const std::string& json);
std::string getExtensionFromMime(const std::string& json);
std::string getMimeFromExtension(const std::string& json);
std::string classifyMediaType(const std::string& json);
