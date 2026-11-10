#pragma once
#include <string>
#include <cstdint>

std::string addFavorite(const std::string& json);
std::string removeFavorite(const std::string& json);
std::string isFavorited(const std::string& json);
std::string getFavorites(const std::string& json);
std::string formatFavoriteIndicator(const std::string& json);
