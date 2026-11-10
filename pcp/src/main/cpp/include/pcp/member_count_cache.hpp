#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getMemberCount(const std::string& json);
std::string incrementMemberCount(const std::string& json);
std::string decrementMemberCount(const std::string& json);
std::string clearMemberCache(const std::string& json);
std::string isCountStale(const std::string& json);
