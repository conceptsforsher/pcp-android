#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string exportRoomKeys(const std::string& json);
std::string importRoomKeys(const std::string& json);
std::string parseKeyFile(const std::string& json);
std::string verifyKeyPassword(const std::string& json);
std::string formatKeyExport(const std::string& json);
