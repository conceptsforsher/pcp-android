#pragma once
#include <string>
#include <cstdint>

std::string parseRateLimitInfo(const std::string& json);
std::string calculateBackoff(const std::string& json);
std::string isRateLimited(const std::string& json);
std::string resetRateLimit(const std::string& json);
std::string formatRateLimitWarning(const std::string& json);
