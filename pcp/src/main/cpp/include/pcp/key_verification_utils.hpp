#pragma once
#include <string>
#include <cstdint>

std::string parseVerificationStart(const std::string& json);
std::string buildVerificationAccept(const std::string& json);
std::string parseKeyVerificationMac(const std::string& json);
std::string validateSasEmoji(const std::string& json);
