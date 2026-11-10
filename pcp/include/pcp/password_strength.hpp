#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string checkStrength(const std::string& json);
std::string getEntropy(const std::string& json);
std::string isCommonPassword(const std::string& json);
std::string getPasswordFeedback(const std::string& json);
std::string minRequiredLength(const std::string& json);
