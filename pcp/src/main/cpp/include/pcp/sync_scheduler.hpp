#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string scheduleNextSync(const std::string& json);
std::string getRetryDelay(const std::string& json);
std::string shouldSyncNow(const std::string& json);
std::string resetSyncTimer(const std::string& json);
std::string formatSyncSchedule(const std::string& json);
