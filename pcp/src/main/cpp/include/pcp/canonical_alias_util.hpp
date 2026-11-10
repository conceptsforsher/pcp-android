#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getCanonicalAlias(const std::string& json);
std::string setCanonicalAlias(const std::string& json);
std::string isCanonicalAliasValid(const std::string& json);
std::string formatAliasForDisplay(const std::string& json);
std::string parseCanonicalEvent(const std::string& json);
