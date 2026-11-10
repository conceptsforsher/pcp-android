#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseConfig(const std::string& json);
std::string getWellKnown(const std::string& json);
std::string getCustomConfig(const std::string& json);
std::string isFeatureEnabled(const std::string& json);
std::string reloadConfig(const std::string& json);
