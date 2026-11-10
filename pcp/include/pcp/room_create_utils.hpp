#pragma once
#include <string>
#include <cstdint>

std::string creator;(const std::string& json);
std::string roomVersion;(const std::string& json);
std::string predecessorRoomId;   // if upgraded(const std::string& json);
std::string RoomCreateInfo parseRoomCreate(const std::string& json);
std::string buildCreateRoomRequest(const std(const std::string& json);
std::string string& name, const std(const std::string& json);
std::string string& topic,(const std::string& json);
std::string bool isDirect, const std(const std::string& json);
std::string string& preset = "private_chat");(const std::string& json);
std::string formatRoomCreateInfo(const RoomCreateInfo& info);(const std::string& json);
