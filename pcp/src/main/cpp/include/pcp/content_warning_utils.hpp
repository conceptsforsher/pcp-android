#pragma once
#include <string>
#include <cstdint>

std::string buildContentWarning(const std::string& reason);
std::string parseContentWarning(const std::string& json);
bool hasContentWarning(const std::string& json);
