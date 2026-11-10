#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string hashMessageContent(const std::string& json);
std::string verifyMessageHash(const std::string& json);
std::string getHashAlgorithm(const std::string& json);
std::string signHashedMessage(const std::string& json);
std::string extractHashFromEvent(const std::string& json);
