#pragma once
#include <string>
#include <cstdint>

std::string requestKeyShare(const std::string& json);
std::string processKeyShare(const std::string& json);
std::string isKeyKnown(const std::string& json);
std::string getMissingSessions(const std::string& json);
std::string formatKeyShareEvent(const std::string& json);
