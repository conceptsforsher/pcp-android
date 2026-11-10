#pragma once
#include <string>
#include <cstdint>

std::string parseFilter(const std::string& json);
std::string buildFilterQuery(const std::string& json);
std::string matchesFilter(const std::string& json);
