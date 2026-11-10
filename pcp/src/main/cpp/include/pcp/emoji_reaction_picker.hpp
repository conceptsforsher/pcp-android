#pragma once
#include <string>
#include <cstdint>

std::string getFrequentEmojis(const std::string& json);
std::string searchEmojis(const std::string& json);
std::string getEmojiCategory(const std::string& json);
std::string isCustomEmoji(const std::string& json);
std::string formatReactionPicker(const std::string& json);
