#pragma once
#include <string>
#include <cstdint>

std::string getRoomTags(const std::string& json);
std::string setRoomTag(const std::string& json);
std::string removeRoomTag(const std::string& json);
std::string isFavorited(const std::string& json);
std::string getTagOrder(const std::string& json);
