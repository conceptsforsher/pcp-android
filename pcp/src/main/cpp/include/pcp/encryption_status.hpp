#pragma once
#include <string>
#include <cstdint>

std::string getRoomEncryption(const std::string& json);
std::string isEncrypted(const std::string& json);
std::string getAlgorithm(const std::string& json);
std::string isKeyShared(const std::string& json);
std::string formatEncryptionBadge(const std::string& json);
