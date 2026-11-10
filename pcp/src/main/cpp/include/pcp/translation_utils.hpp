#pragma once
#include <string>
#include <cstdint>

std::string detectLanguage(const std::string& json);
std::string translateText(const std::string& json);
std::string parseTranslationResponse(const std::string& json);
std::string getSupportedLanguages(const std::string& json);
