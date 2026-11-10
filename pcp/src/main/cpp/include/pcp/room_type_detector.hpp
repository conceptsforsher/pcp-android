#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseRoomType(const std::string& json);
std::string isSpace(const std::string& json);
std::string isDirect(const std::string& json);
std::string isGroupRoom(const std::string& json);
std::string formatRoomTypeLabel(const std::string& json);
