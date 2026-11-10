#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string createAccount(const std::string& json);
std::string signMessage(const std::string& json);
std::string verifySignature(const std::string& json);
std::string generateKeys(const std::string& json);
std::string getIdentityKey(const std::string& json);
