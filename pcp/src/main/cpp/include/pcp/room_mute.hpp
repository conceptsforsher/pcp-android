#pragma once
#include <string>
#include <cstdint>

std::string muteRoom(const std::string& json);
std::string unmuteRoom(const std::string& json);
std::string isMuted(const std::string& json);
std::string getMuteUntil(const std::string& json);
std::string formatMuteStatus(const std::string& json);
