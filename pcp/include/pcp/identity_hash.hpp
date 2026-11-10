#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string hashIdentifier(const std::string& json);
std::string lookupHash(const std::string& json);
std::string verifyHashedBinding(const std::string& json);
std::string buildInviteWithHash(const std::string& json);
std::string isHashValid(const std::string& json);
