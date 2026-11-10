#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string captureCrash(const std::string& json);
std::string formatStackTrace(const std::string& json);
std::string getDeviceInfo(const std::string& json);
std::string buildCrashReport(const std::string& json);
std::string sendCrashReport(const std::string& json);
