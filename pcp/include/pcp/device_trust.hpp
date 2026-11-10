#pragma once
#include <string>
#include <cstdint>

std::string deviceId;(const std::string& json);
std::string userId;(const std::string& json);
std::string lastVerifiedDate;(const std::string& json);
std::string trustLevelToString(DeviceTrustLevel level);(const std::string& json);
std::string trustLevelToEmoji(DeviceTrustLevel level);(const std::string& json);
std::string trustLevelToColor(DeviceTrustLevel level);(const std::string& json);
std::string DeviceTrustInfo parseDeviceTrust(const std(const std::string& json);
std::string string& deviceId, const std(const std::string& json);
std::string string& userId,(const std::string& json);
std::string const std(const std::string& json);
std::string string& cryptoJson);(const std::string& json);
std::string formatDeviceTrust(const DeviceTrustInfo& info);(const std::string& json);
