#pragma once
#include <string>
#include <cstdint>
#include <vector>

std::string getAvatarUrl(const std::string& json);
std::string setAvatarUrl(const std::string& json);
std::string removeAvatar(const std::string& json);
std::string cropAvatar(const std::string& json);
std::string formatAvatarEvent(const std::string& json);
