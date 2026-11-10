#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getFontScale(const std::string& json);
std::string setFontScale(const std::string& json);
std::string getAvailableFonts(const std::string& json);
std::string applyFont(const std::string& json);
std::string formatFontPreview(const std::string& json);
