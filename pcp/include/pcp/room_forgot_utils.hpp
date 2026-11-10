#pragma once
#include <string>
#include <cstdint>

std::string parseForgotten(const std::string& json);
std::string markAsForgotten(const std::string& json);
std::string unforgetRoom(const std::string& json);
std::string isForgotten(const std::string& json);
std::string formatForgotStatus(const std::string& json);
