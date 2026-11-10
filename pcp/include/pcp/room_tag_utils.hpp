#pragma once
#include <string>
#include <cstdint>

std::string parseRoomTags(const std::string& json);
std::string addRoomTag(const std::string& json);
std::string removeRoomTag(const std::string& json);
std::string buildTagEvent(const std::string& json);
