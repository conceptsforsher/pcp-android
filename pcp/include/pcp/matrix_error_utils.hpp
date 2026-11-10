#pragma once
#include <string>
#include <cstdint>

std::string formatMatrixError(const std(const std::string& json);
std::string string& errcode, const std(const std::string& json);
std::string string& error);(const std::string& json);
std::string bool isRetryableError(const std(const std::string& json);
std::string string& errcode);(const std::string& json);
std::string bool isAuthError(const std(const std::string& json);
std::string string& errcode);(const std::string& json);
std::string bool isRateLimitError(const std(const std::string& json);
std::string string& errcode);(const std::string& json);
std::string getFriendlyErrorMessage(const std(const std::string& json);
std::string string& errcode);(const std::string& json);
