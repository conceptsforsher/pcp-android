#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string sendTyping(const std::string& json);
std::string stopTyping(const std::string& json);
std::string getTypingTimeout(const std::string& json);
std::string isCurrentlyTyping(const std::string& json);
std::string formatTypingIndicator(const std::string& json);
