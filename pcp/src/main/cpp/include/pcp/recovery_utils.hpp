#pragma once
#include <string>
#include <cstdint>

std::string parseRecoveryKey(const std::string& json);
std::string generateRecoveryPrompt(const std::string& json);
std::string verifyRecoveryPhrase(const std::string& json);
std::string buildRecoveryEvent(const std::string& json);
std::string estimateRecoveryStrength(const std::string& json);
