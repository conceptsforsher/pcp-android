#pragma once
#include <string>
#include <cstdint>

std::string parseUpgradeEvent(const std::string& json);
std::string getRecommendedVersion(const std::string& json);
std::string buildUpgradeTombstone(const std::string& json);
std::string canUpgrade(const std::string& json);
std::string formatUpgradeNotice(const std::string& json);
