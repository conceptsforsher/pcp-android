#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string compressImage(const std::string& json);
std::string compressVideo(const std::string& json);
std::string getCompressionRatio(const std::string& json);
std::string originalSize(const std::string& json);
std::string compressedSize(const std::string& json);
