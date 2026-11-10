#pragma once
#include <string>
#include <cstdint>

std::string parseCropRegion(const std::string& json);
std::string computeCrop(const std::string& json);
std::string applyCrop(const std::string& json);
std::string validateAspectRatio(const std::string& json);
std::string formatCropParams(const std::string& json);
