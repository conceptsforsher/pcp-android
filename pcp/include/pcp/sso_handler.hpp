#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseSsoRedirect(const std::string& json);
std::string buildSsoUrl(const std::string& json);
std::string extractTokenFromUrl(const std::string& json);
std::string getProviderName(const std::string& json);
std::string formatSsoLoading(const std::string& json);
