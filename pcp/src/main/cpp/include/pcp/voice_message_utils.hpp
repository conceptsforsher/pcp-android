#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseVoiceInfo(const std::string& json);
std::string getDuration(const std::string& json);
std::string isPlayed(const std::string& json);
std::string formatWaveform(const std::string& json);
std::string transcodeAudio(const std::string& json);
