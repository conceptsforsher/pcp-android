#pragma once
#include <string>
#include <cstdint>

std::string parseFilterQuery(const std::string& json);
std::string buildSyncFilter(const std::string& json);
std::string mergeFilterSets(const std::string& json);
std::string getDefaultFilter(const std::string& json);
std::string validateFilterJson(const std::string& json);
