#pragma once
#include <string>
#include <cstdint>

std::string lookupEmail(const std::string& json);
std::string lookupPhone(const std::string& json);
std::string getHashedId(const std::string& json);
std::string parseLookupResult(const std::string& json);
std::string formatLookupNotice(const std::string& json);
