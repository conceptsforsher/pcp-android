#pragma once
#include <string>
#include <cstdint>

std::string parseDisplayName(const std::string& json);
std::string formatDisplayName(const std::string& json);
std::string buildDisplayNameUpdate(const std::string& json);
