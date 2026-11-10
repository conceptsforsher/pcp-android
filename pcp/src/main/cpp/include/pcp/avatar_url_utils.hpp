#pragma once
#include <string>
#include <cstdint>

std::string parseAvatarUrl(const std::string& json);
std::string buildAvatarUpdateEvent(const std::string& json);
