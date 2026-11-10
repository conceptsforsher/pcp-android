#pragma once
#include <string>
#include <cstdint>

std::string getUserBadge(const std::string& json);
std::string getPowerBadge(const std::string& json);
std::string isAdmin(const std::string& json);
std::string isModerator(const std::string& json);
std::string formatBadgeHtml(const std::string& json);
