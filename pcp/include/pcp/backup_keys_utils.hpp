#pragma once
#include <string>
#include <cstdint>

std::string generateBackupKey(const std::string& json);
std::string verifyBackupKey(const std::string& json);
std::string encryptBackupData(const std::string& json);
