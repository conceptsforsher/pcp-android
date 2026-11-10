#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string aggregateReactions(const std::string& json);
std::string getReactionCount(const std::string& json);
std::string whoReactedWith(const std::string& json);
std::string isSelfReaction(const std::string& json);
std::string formatReactionSummary(const std::string& json);
