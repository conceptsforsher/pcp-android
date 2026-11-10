#pragma once
#include <string>
#include <cstdint>

std::string getProviders(const std::string& json);
std::string selectProvider(const std::string& json);
std::string parseProviderBrand(const std::string& json);
std::string buildSsoUrl(const std::string& json);
std::string formatProviderList(const std::string& json);
