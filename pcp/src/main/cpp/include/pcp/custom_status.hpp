#pragma once
#include <string>
#include <cstdint>

std::string parseStatus(const std::string& json);
std::string setStatus(const std::string& json);
std::string expireStatus(const std::string& json);
std::string getUserStatusText(const std::string& json);
std::string formatStatusEmoji(const std::string& json);
