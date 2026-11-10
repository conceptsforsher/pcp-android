#pragma once
#include <string>
#include <cstdint>

std::string detectBridge(const std::string& json);
std::string getBridgeType(const std::string& json);
std::string isBridgedRoom(const std::string& json);
std::string formatBridgeNotice(const std::string& json);
std::string parseBridgeState(const std::string& json);
