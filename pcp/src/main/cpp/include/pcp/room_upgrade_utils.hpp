#pragma once
#include <string>
#include <cstdint>

std::string parseUpgrade(const std::string& json);
std::string getReplacementRoom(const std::string& json);
std::string isTombstoned(const std::string& json);
std::string buildUpgradeLink(const std::string& json);
std::string formatUpgradePrompt(const std::string& json);
