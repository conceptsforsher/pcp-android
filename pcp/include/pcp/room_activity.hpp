#pragma once
#include <string>
#include <cstdint>

std::string getLastActivity(const std::string& json);
std::string isRoomActive(const std::string& json);
std::string getMessageFrequency(const std::string& json);
std::string formatActivitySummary(const std::string& json);
std::string sortByActivity(const std::string& json);
