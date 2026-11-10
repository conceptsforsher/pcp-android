#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string searchEmoji(const std::string& json);
std::string getEmojiByCategory(const std::string& json);
std::string getEmojiVariations(const std::string& json);
std::string isValidEmojiCodepoint(const std::string& json);
std::string formatEmojiPicker(const std::string& json);
