#pragma once
#include <string>
#include <cstdint>

std::string verifyDeviceKey(const std::string& json);
std::string verifyCrossSign(const std::string& json);
std::string checkKeyTrust(const std::string& json);
std::string getTrustLevel(const std::string& json);
std::string formatTrustBadge(const std::string& json);
