#pragma once
#include <string>
#include <cstdint>
namespace pcp {
bool shouldRetryMessage(const std::string& errorCode, int retryCount, int maxRetries=3);
int64_t getRetryDelayMs(int retryCount);
std::string formatRetryStatus(int retryCount, int maxRetries);
}
