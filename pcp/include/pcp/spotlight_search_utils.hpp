#pragma once
#include <string>
#include <cstdint>

std::string searchAll(const std::string& json);
std::string getRecentSearches(const std::string& json);
std::string clearRecent(const std::string& json);
std::string indexContent(const std::string& json);
std::string formatSearchResult(const std::string& json);
