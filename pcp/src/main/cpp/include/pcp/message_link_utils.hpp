#pragma once
#include <string>
#include <cstdint>

std::string buildMessageLink(const std::string& roomId, const std::string& eventId);
std::string buildUserLink(const std::string& userId);
std::string buildRoomLink(const std::string& roomId, const std::string& viaServer="");
