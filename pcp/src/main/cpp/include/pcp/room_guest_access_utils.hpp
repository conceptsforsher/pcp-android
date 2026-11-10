#pragma once
#include <string>
#include <cstdint>

std::string parseGuestAccess(const std::string& json);
std::string isGuestAllowed(const std::string& json);
std::string buildGuestAccessEvent(const std::string& json);
