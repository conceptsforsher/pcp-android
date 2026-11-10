#pragma once
#include <string>
#include <cstdint>

std::string buildCustomEmojiImage(const std::string& json);
std::string extractEmojiShortcode(const std::string& json);
std::string replaceEmojiShortcodes(const std::string& json);
