#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string blockUser(const std::string& json);
std::string unblockUser(const std::string& json);
std::string isBlocked(const std::string& json);
std::string getBlockList(const std::string& json);
std::string formatBlockNotice(const std::string& json);
