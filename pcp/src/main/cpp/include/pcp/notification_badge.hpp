#pragma once
#include <string>
#include <cstdint>

std::string calculateBadgeCount(const std::string& json);
std::string isHighlighted(const std::string& json);
std::string isNoisy(const std::string& json);
std::string formatBadge(const std::string& json);
std::string resetBadgeCount(const std::string& json);
