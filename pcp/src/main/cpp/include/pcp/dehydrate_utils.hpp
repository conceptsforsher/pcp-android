#pragma once
#include <string>
#include <cstdint>

std::string parseDehydratedDevice(const std::string& json);
std::string buildDehydrationEvent(const std::string& json);
std::string checkDehydrationStatus(const std::string& json);
std::string rehydrateDevice(const std::string& json);
std::string exportPickleKey(const std::string& json);
