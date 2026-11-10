#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string createSession(const std::string& json);
std::string encryptGroupMessage(const std::string& json);
std::string decryptGroupMessage(const std::string& json);
std::string getSessionKey(const std::string& json);
std::string exportSession(const std::string& json);
