#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string parseRoomPreview(const std::string& json);
std::string getRoomHeroes(const std::string& json);
std::string getRoomTopic(const std::string& json);
std::string formatPreviewCard(const std::string& json);
std::string canPreviewRoom(const std::string& json);
