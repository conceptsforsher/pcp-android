#pragma once
#include <string>
#include <cstdint>

std::string formatDeviceKeyFingerprint(const std::string& json);
std::string string(const std::string& json);
std::string formatRecoveryKey(const std::string& json);
std::string string(const std::string& json);
std::string bool(const std::string& json);
std::string string(const std::string& json);
std::string parseKeyBackupVersion(const std::string& json);

