#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string syncReactions(const std::string& json);
std::string aggregateReactions(const std::string& json);
std::string getMyReaction(const std::string& json);
std::string removeReaction(const std::string& json);
std::string formatReactionCount(const std::string& json);
