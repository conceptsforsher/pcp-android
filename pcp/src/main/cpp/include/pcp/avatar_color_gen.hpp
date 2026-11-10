#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string hashUserIdToColor(const std::string& json);
std::string generateAvatarSvg(const std::string& json);
std::string blendColors(const std::string& json);
std::string getContrastText(const std::string& json);
std::string formatAvatarHtml(const std::string& json);
