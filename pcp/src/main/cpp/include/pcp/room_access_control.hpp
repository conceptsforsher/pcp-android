#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseAccessRules(const std::string& json);
std::string canUserJoin(const std::string& json);
std::string isRestricted(const std::string& json);
std::string buildAccessEvent(const std::string& json);
std::string getRecommendedAccessLevel(const std::string& json);
