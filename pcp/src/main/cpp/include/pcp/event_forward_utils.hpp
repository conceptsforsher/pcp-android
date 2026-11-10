#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string buildForwardRequest(const std::vector<std::string>& roomIds);
std::string buildForwardContent(const std::string& eventId, const std::string& roomId);
