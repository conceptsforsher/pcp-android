#pragma once
#include <string>
#include <cstdint>

std::string isBiometricAvailable(const std::string& json);
std::string authenticate(const std::string& json);
std::string getBiometricType(const std::string& json);
std::string canUseBiometrics(const std::string& json);
std::string formatBiometricPrompt(const std::string& json);
