#pragma once
#include <string>
#include <cstdint>

std::string parseFilter(const std::string& json);
std::string applyFilterToTimeline(const std::string& json);
std::string getFilteredCount(const std::string& json);
std::string isEventFiltered(const std::string& json);
std::string formatFilterLabel(const std::string& json);
