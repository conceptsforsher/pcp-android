#pragma once
#include <string>
#include <cstdint>

std::string buildReactionRelation(const std::string& eventId, const std::string& emoji);
std::string buildEditRelation(const std::string& eventId);
std::string buildThreadRelation(const std::string& eventId, bool fallingBack = false);
