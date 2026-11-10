#pragma once
#include <string>
#include <cstdint>

std::string parseEventAge(const std::string& json);
std::string calculateAge(const std::string& json);
std::string isExpiredEvent(const std::string& json);
