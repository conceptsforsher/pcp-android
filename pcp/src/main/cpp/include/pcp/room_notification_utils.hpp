#pragma once
#include <string>
#include <cstdint>

std::string buildRoomNotifOverride(const std::string& roomId, const std::string& level);
std::string buildRoomNotifRemove(const std::string& roomId);
std::string formatNotifLevel(const std::string& level);
