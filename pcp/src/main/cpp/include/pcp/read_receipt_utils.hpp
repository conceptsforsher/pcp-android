#pragma once
#include <string>
#include <cstdint>

std::string parseReadReceipt(const std::string& json);
std::string buildReceiptEvent(const std::string& json);
std::string getReadPosition(const std::string& json);
std::string getUnreadCount(const std::string& json);
std::string formatReceiptAck(const std::string& json);
