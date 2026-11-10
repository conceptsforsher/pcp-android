#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getPowerLevelColor(const std::string& json);
std::string getPowerLevelIcon(const std::string& json);
std::string formatPowerBadge(const std::string& json);
std::string getPowerDescription(const std::string& json);
std::string parsePowerLevels(const std::string& json);
