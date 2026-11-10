#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string dispatchEvent(const std::string& json);
std::string routeToHandler(const std::string& json);
std::string prioritySort(const std::string& json);
std::string canHandle(const std::string& json);
std::string getRegisteredHandlers(const std::string& json);
