#pragma once
#include <string>
#include <cstdint>

std::string userId(const std::string& json);
std::string displayName(const std::string& json);
std::string avatarUrl(const std::string& json);
std::string buildProfileRequest(const std::string& json);
std::string UserProfile(const std::string& json);
std::string string(const std::string& json);
std::string string(const std::string& json);
std::string buildProfileUpdate(const std::string& json);
std::string buildDisplayNameChange(const std::string& json);
std::string string(const std::string& json);
std::string buildAvatarUrlChange(const std::string& json);
std::string string(const std::string& json);
std::string formatProfileSummary(const std::string& json);

