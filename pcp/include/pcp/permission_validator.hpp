#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string checkRoomPermission(const std::string& json);
std::string getPowerLevel(const std::string& json);
std::string canSendMessage(const std::string& json);
std::string canInviteUser(const std::string& json);
std::string formatPermissionDenied(const std::string& json);
