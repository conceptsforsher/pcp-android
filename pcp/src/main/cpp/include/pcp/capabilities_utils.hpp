#pragma once
#include <string>
#include <cstdint>

std::string parseCapabilities(const std::string& json);
std::string getServerCap(const std::string& json);
std::string getRoomCap(const std::string& json);
std::string checkCapability(const std::string& json);
std::string buildCapRequest(const std::string& json);
