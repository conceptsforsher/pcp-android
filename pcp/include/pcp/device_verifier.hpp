#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string startVerification(const std::string& json);
std::string acceptVerification(const std::string& json);
std::string cancelVerification(const std::string& json);
std::string parseVerificationEvent(const std::string& json);
std::string formatVerificationEmoji(const std::string& json);
