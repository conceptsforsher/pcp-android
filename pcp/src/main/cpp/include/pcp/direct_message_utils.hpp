#pragma once
#include <string>
#include <cstdint>

std::string buildDirectMessageFlag(const std::string& userId, const std::string& roomId);
bool isDirectMessage(const std::string& accountDataJson, const std::string& roomId);
