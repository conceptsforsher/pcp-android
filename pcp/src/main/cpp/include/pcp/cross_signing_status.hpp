#pragma once
#include <string>
#include <cstdint>

std::string getMasterKeyStatus(const std::string& json);
std::string isCrossSigningReady(const std::string& json);
std::string getSSSSStatus(const std::string& json);
std::string formatCrossSigningBanner(const std::string& json);
std::string needsSetup(const std::string& json);
