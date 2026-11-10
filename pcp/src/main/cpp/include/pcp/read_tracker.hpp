#pragma once
#include <string>
#include <cstdint>

std::string markRead(const std::string& json);
std::string isRead(const std::string& json);
std::string getUnreadPosition(const std::string& json);
std::string getReadReceipts(const std::string& json);
std::string formatReadStatus(const std::string& json);
