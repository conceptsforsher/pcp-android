#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseAlias(const std::string& json);
std::string addRoomAlias(const std::string& json);
std::string removeRoomAlias(const std::string& json);
std::string getLocalAliases(const std::string& json);
std::string resolveRoomAlias(const std::string& json);
