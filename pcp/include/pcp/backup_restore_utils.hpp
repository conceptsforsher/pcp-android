#pragma once
#include <string>
#include <cstdint>

std::string parseBackup(const std::string& json);
std::string restoreFromBackup(const std::string& json);
std::string validateBackup(const std::string& json);
std::string buildRestoreEvent(const std::string& json);
std::string formatRestoreProgress(const std::string& json);
