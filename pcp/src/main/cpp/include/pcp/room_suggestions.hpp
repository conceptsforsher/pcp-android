#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getSuggestedRooms(const std::string& json);
std::string getTrendingRooms(const std::string& json);
std::string getPopularRooms(const std::string& json);
std::string filterByLanguage(const std::string& json);
std::string formatRoomSuggestion(const std::string& json);
