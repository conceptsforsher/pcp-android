#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseLanguage(const std::string& json);
std::string getSupportedLanguages(const std::string& json);
std::string setAppLanguage(const std::string& json);
std::string getDefaultLanguage(const std::string& json);
std::string formatLanguageSelector(const std::string& json);
