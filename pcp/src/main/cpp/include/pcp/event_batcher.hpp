#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string addToBatch(const std::string& json);
std::string flushBatch(const std::string& json);
std::string batchSize(const std::string& json);
std::string getPendingEvents(const std::string& json);
std::string clearBatch(const std::string& json);
