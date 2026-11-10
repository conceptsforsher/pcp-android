#pragma once
#include <string>
#include <cstdint>

std::string parseServerNotice(const std::string& json);
std::string buildAdminCommand(const std::string& json);
std::string validateAdminToken(const std::string& json);
std::string checkAdminPermission(const std::string& json);
std::string formatAdminResponse(const std::string& json);
