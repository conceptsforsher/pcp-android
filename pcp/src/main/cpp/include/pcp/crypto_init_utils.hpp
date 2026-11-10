#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string initializeCrypto(const std::string& json);
std::string getCryptoVersion(const std::string& json);
std::string isCryptoAvailable(const std::string& json);
std::string resetCrypto(const std::string& json);
std::string formatCryptoStatus(const std::string& json);
