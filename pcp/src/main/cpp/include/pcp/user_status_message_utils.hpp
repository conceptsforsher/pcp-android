#pragma once
#include <string>
namespace pcp {
std::string buildStatusMessage(const std::string& status, const std::string& emoji="");
std::string parseStatusMessage(const std::string& json);
bool isStatusExpired(int64_t setAtMs, int64_t maxAgeMs=86400000);
}
