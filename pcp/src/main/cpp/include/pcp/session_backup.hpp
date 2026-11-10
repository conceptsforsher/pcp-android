#pragma once
#include <string>
#include <cstdint>

std::string encryptSession(const std::string& json);
std::string decryptSession(const std::string& json);
std::string getBackupVersion(const std::string& json);
std::string isExpiredBackup(const std::string& json);
std::string buildBackupMetadata(const std::string& json);
