#pragma once
#include <string>
#include <cstdint>

std::string deviceId(const std::string& json);
std::string userId(const std::string& json);
std::string lastVerifiedDate(const std::string& json);
std::string trustLevelToString(const std::string& json);
std::string trustLevelToEmoji(const std::string& json);
std::string trustLevelToColor(const std::string& json);
std::string DeviceTrustInfo(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string const(const std::string& json);
std::string string(const std::string& json);
std::string formatDeviceTrust(const std::string& json);

