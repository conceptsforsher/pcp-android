#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string checkPermission(const std::string& json);
std::string requestPermission(const std::string& json);
std::string getDeniedCount(const std::string& json);
std::string isPermanentlyDenied(const std::string& json);
