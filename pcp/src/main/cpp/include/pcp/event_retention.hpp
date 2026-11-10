#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string shouldRetain(const std::string& json);
std::string getRetentionPeriod(const std::string& json);
std::string pruneOldEvents(const std::string& json);
std::string calculateAge(const std::string& json);
std::string buildRetentionPolicy(const std::string& json);
