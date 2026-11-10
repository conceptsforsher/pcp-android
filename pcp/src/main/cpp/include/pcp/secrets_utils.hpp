#pragma once
#include <string>
#include <cstdint>

std::string encryptSecret(const std::string& json);
std::string decryptSecret(const std::string& json);
std::string hashSecret(const std::string& json);
