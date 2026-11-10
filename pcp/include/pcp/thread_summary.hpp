#pragma once
#include <string>
#include <cstdint>

std::string getThreadCount(const std::string& json);
std::string getLatestThreadEvent(const std::string& json);
std::string isParticipating(const std::string& json);
std::string formatThreadPreview(const std::string& json);
std::string getUnreadThreads(const std::string& json);
