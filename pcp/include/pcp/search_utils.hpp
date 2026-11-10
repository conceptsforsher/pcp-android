#pragma once
#include <string>
#include <cstdint>

std::string tokenizeQuery(const std::string& json);
std::string buildSearchBody(const std::string& json);
std::string parseSearchResults(const std::string& json);
std::string rankSearchResults(const std::string& json);
std::string applySearchFilter(const std::string& json);
