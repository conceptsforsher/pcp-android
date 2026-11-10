#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string backupKeys(const std::string& json);
std::string restoreKeys(const std::string& json);
std::string getBackupVersion(const std::string& json);
std::string deleteBackup(const std::string& json);
std::string formatBackupStatus(const std::string& json);
