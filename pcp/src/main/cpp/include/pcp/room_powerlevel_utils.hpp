#pragma once
#include <string>
#include <cstdint>

std::string parsePowerLevels(const std::string& json);
std::string buildPowerLevelEvent(const std::string& json);
std::string getUserPowerLevel(const std::string& json);
std::string setUserPowerLevel(const std::string& json);
std::string defaultPowerLevels(const std::string& json);
