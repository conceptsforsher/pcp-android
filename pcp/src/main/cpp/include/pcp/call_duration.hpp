#pragma once
#include <string>
#include <cstdint>

std::string parseCallStart(const std::string& json);
std::string formatDuration(const std::string& json);
std::string isCallActive(const std::string& json);
std::string getCallType(const std::string& json);
std::string buildCallSummary(const std::string& json);
