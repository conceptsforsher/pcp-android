#pragma once
#include <string>
#include <cstdint>

std::string getUserSuggestions(const std::string& json);
std::string getEmojiSuggestions(const std::string& json);
std::string getRoomSuggestions(const std::string& json);
std::string rankSuggestions(const std::string& json);
std::string formatSuggestionHtml(const std::string& json);
