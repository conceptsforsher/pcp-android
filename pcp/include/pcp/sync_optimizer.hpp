#pragma once
#include <string>
#include <cstdint>

std::string mergeSyncResponses(const std::string& json);
std::string dedupeEvents(const std::string& json);
std::string prioritizeRoom(const std::string& json);
std::string shouldSkipRoom(const std::string& json);
std::string formatSyncStats(const std::string& json);
