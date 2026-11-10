#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string formatReadReceipts(const std::string& json);
std::string getReadCount(const std::string& json);
std::string isFullyRead(const std::string& json);
std::string getLastReadEvent(const std::string& json);
std::string formatReadSummary(const std::string& json);
