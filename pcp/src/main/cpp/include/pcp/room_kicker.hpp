#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string canKickUser(const std::string& json);
std::string buildKickEvent(const std::string& json);
std::string parseKickResponse(const std::string& json);
std::string getKickReason(const std::string& json);
std::string formatKickNotice(const std::string& json);
