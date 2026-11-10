#pragma once
#include <string>
#include <cstdint>

std::string getSupportedVersions(const std::string& json);
std::string checkFeature(const std::string& json);
std::string isDeprecated(const std::string& json);
std::string getMinimumVersion(const std::string& json);
std::string formatServerInfo(const std::string& json);
