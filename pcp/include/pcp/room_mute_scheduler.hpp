#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string scheduleMute(const std::string& json);
std::string cancelMute(const std::string& json);
std::string getMuteUntil(const std::string& json);
std::string isMuted(const std::string& json);
std::string formatMuteStatus(const std::string& json);
