#pragma once
#include <string>
#include <cstdint>

std::string isValidEmoji(const std::string& json);
std::string searchEmoji(const std::string& json);
std::string getEmojiCategory(const std::string& json);
std::string parseEmojiCodepoints(const std::string& json);
std::string renderEmojiHtml(const std::string& json);
