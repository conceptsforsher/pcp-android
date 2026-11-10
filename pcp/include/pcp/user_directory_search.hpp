#pragma once
#include <string>
#include <cstdint>

std::string searchUsers(const std::string& json);
std::string parseSearchResults(const std::string& json);
std::string isExactMatch(const std::string& json);
std::string rankByRelevance(const std::string& json);
std::string formatSearchResult(const std::string& json);
