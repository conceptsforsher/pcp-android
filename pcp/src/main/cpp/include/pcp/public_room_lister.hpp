#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parsePublicRooms(const std::string& json);
std::string filterBySearch(const std::string& json);
std::string sortByPopularity(const std::string& json);
std::string paginateRooms(const std::string& json);
std::string formatRoomDirectoryEntry(const std::string& json);
