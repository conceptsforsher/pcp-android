#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string syncPresence(const std::string& json);
std::string getUserPresence(const std::string& json);
std::string setUserPresence(const std::string& json);
std::string getLastActiveAgo(const std::string& json);
std::string formatPresenceText(const std::string& json);
