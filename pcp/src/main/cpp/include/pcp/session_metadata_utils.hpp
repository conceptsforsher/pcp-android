#pragma once
#include <string>
#include <cstdint>

std::string parseSessionInfo(const std::string& json);
std::string buildSessionMetadata(const std::string& json);
std::string getLastActiveTimestamp(const std::string& json);
std::string formatDeviceFingerprint(const std::string& json);
std::string hashSessionId(const std::string& json);
