#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string clearNotification(const std::string& json);
std::string dismissAll(const std::string& json);
std::string isNotificationActive(const std::string& json);
std::string getActiveNotifications(const std::string& json);
std::string formatClearAction(const std::string& json);
