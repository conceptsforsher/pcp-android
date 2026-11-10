#pragma once
#include <string>
#include <cstdint>

std::string parseVoiceBroadcastInfo(const std::string& json);
std::string buildVoiceBroadcastEvent(const std::string& json);
std::string getVoiceBroadcastState(const std::string& json);
std::string canStartVoiceBroadcast(const std::string& json);
