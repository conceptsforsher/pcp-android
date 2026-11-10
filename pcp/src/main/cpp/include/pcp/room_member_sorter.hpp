#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string sortByName(const std::string& json);
std::string sortByPowerLevel(const std::string& json);
std::string sortByJoinDate(const std::string& json);
std::string filterOnline(const std::string& json);
std::string formatMemberEntry(const std::string& json);
