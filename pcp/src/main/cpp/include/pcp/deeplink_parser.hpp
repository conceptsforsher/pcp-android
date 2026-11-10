#pragma once
#include <string>
#include <cstdint>

std::string parseMatrixTo(const std::string& json);
std::string parseMatrixLink(const std::string& json);
std::string isValidDeeplink(const std::string& json);
std::string extractRoomId(const std::string& json);
std::string formatDeeplinkRedirect(const std::string& json);
