#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string lookupIdentity(const std::string& json);
std::string bindIdentity(const std::string& json);
std::string unbindIdentity(const std::string& json);
std::string getBindings(const std::string& json);
std::string formatIdentityCard(const std::string& json);
