#pragma once
#include <string>
#include <cstdint>

std::string calculateOffset(const std::string& json);
std::string buildPaginationToken(const std::string& json);
std::string parsePaginationToken(const std::string& json);
std::string mergePaginatedResults(const std::string& json);
