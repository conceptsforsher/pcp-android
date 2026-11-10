#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseTheme(const std::string& json);
std::string applyDarkMode(const std::string& json);
std::string getAccentColor(const std::string& json);
std::string isSystemTheme(const std::string& json);
std::string formatThemeCss(const std::string& json);
