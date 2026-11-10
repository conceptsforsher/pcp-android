#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string checkHeartbeat(const std::string& json);
std::string lastActivityAge(const std::string& json);
std::string shouldTerminate(const std::string& json);
std::string resetWatchdog(const std::string& json);
