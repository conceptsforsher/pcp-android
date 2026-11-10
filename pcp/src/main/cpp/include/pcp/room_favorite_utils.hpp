#pragma once
#include <string>
namespace pcp {
std::string buildFavoriteTag(bool favorite, double order=0.0);
std::string buildLowPriorityTag(bool lowPriority);
bool isFavoritedTag(const std::string& json);
bool isLowPriorityTag(const std::string& json);
}
