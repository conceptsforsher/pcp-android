#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseBackupVersion(const std::string& json);
std::string createBackupVersion(const std::string& json);
std::string deleteBackupVersion(const std::string& json);
std::string getLatestVersion(const std::string& json);
std::string formatBackupInfo(const std::string& json);
