#pragma once
#include <string>
#include <cstdint>

std::string encryptSecret(const std::string& json);
std::string decryptSecret(const std::string& json);
std::string parseSecretStorageKey(const std::string& json);
std::string buildSecretStorageEvent(const std::string& json);
std::string getDefaultKeyId(const std::string& json);
