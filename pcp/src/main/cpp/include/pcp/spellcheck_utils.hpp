#pragma once
#include <string>
#include <cstdint>

std::string loadDictionary(const std::string& json);
std::string checkSpelling(const std::string& json);
std::string getSuggestions(const std::string& json);
std::string tokenizeForSpelling(const std::string& json);
std::string getMisspelledWords(const std::string& json);
