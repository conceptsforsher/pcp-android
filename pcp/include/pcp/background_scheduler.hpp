#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string scheduleTask(const std::string& json);
std::string cancelTask(const std::string& json);
std::string getNextRunTime(const std::string& json);
std::string taskExists(const std::string& json);
std::string listTasks(const std::string& json);
