#pragma once
#include <string>
#include <cstdint>

std::string trackPushReceived(const std::string& json);
std::string trackPushDismissed(const std::string& json);
std::string trackPushOpened(const std::string& json);
std::string getPushStats(const std::string& json);
std::string dailyPushSummary(const std::string& json);
