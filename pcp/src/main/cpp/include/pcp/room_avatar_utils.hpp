#pragma once
#include <string>
#include <cstdint>

std::string generateRoomInitials(const std::string& roomName);
std::string getRoomColor(const std::string& roomId);
std::string formatRoomAvatarUrl(const std::string& mxcUrl, const std::string& homeserver,const std::string& roomName, const std::string& roomId);
bool hasCustomRoomAvatar(const std::string& stateJson);
std::string buildRoomAvatarChange(const std::string& mxcUrl);
