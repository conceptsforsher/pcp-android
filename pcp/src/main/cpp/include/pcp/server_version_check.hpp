#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseServerVersion(const std::string& json);
std::string isVersionSupported(const std::string& json);
std::string compareVersions(const std::string& json);
std::string getMinRequiredVersion(const std::string& json);
std::string formatVersionMismatch(const std::string& json);
