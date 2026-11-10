#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string discoverHomeserver(const std::string& json);
std::string parseWellKnown(const std::string& json);
std::string getIdentityServer(const std::string& json);
std::string validateServerCert(const std::string& json);
std::string cacheDiscovery(const std::string& json);
