#pragma once
#include <string>
#include <cstdint>

std::string parseOneTimeKey(const std::string& json);
std::string buildSignedKey(const std::string& json);
std::string verifyKeySignature(const std::string& json);
std::string countAvailableKeys(const std::string& json);
std::string selectBestKey(const std::string& json);
