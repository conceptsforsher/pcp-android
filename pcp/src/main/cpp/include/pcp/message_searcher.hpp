#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string searchMessages(const std::string& json);
std::string highlightMatches(const std::string& json);
std::string tokenizeContent(const std::string& json);
std::string buildSearchIndex(const std::string& json);
std::string rankResults(const std::string& json);
