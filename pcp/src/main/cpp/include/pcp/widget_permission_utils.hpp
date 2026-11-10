#pragma once
#include <string>
#include <cstdint>

std::string parsePermissions(const std::string& json);
std::string checkPermission(const std::string& json);
std::string buildPermissionRequest(const std::string& json);
