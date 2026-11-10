#pragma once
#include <string>
#include <cstdint>

std::string parseApiCall(const std::string& json);
std::string buildApiResponse(const std::string& json);
std::string validateApiMethod(const std::string& json);
std::string getSupportedApis(const std::string& json);
std::string formatApiError(const std::string& json);
