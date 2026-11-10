#pragma once
#include <string>
#include <cstdint>

std::string parseAlias(const std::string& json);
std::string isValidAlias(const std::string& json);
std::string isLocalAlias(const std::string& json);
std::string getServerFromAlias(const std::string& json);
std::string normalizeAlias(const std::string& json);
