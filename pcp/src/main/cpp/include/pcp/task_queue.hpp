#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string enqueueTask(const std::string& json);
std::string dequeueTask(const std::string& json);
std::string peekNext(const std::string& json);
std::string taskCount(const std::string& json);
std::string cancelAll(const std::string& json);
