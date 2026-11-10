#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string startBroadcast(const std::string& json);
std::string stopBroadcast(const std::string& json);
std::string getBroadcastState(const std::string& json);
std::string getListenerCount(const std::string& json);
std::string formatBroadcastChunk(const std::string& json);
