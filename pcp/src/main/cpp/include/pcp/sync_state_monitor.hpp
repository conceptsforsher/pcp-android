#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getSyncState(const std::string& json);
std::string isSyncing(const std::string& json);
std::string getSyncProgress(const std::string& json);
std::string getLastSyncTime(const std::string& json);
std::string formatSyncStatus(const std::string& json);
