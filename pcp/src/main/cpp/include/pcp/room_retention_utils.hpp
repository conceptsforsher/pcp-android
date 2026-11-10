#pragma once
#include <string>
#include <cstdint>
namespace pcp {
int64_t parseRetentionPeriod(const std::string& stateJson);
bool hasMessageRetention(const std::string& stateJson);
std::string buildRetentionEvent(int64_t maxLifetimeMs);
}
