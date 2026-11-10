#pragma once
#include <string>
#include <cstdint>

std::string validateUserId(const std::string& json);
std::string parseUserId(const std::string& json);
std::string buildUserId(const std::string& json);
std::string formatMxid(const std::string& json);
std::string normalizeUserId(const std::string& json);
