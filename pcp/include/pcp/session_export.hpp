#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string exportSession(const std::string& json);
std::string importSession(const std::string& json);
std::string validateSessionFile(const std::string& json);
std::string encryptSessionBundle(const std::string& json);
std::string decryptSessionBundle(const std::string& json);
