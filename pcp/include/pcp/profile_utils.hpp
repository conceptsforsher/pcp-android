#pragma once
#include <string>
#include <cstdint>

std::string userId;(const std::string& json);
std::string displayName;(const std::string& json);
std::string avatarUrl;(const std::string& json);
std::string buildProfileRequest();(const std::string& json);
std::string UserProfile parseProfile(const std(const std::string& json);
std::string string& json, const std(const std::string& json);
std::string string& userId);(const std::string& json);
std::string buildProfileUpdate(const UserProfile& profile);(const std::string& json);
std::string buildDisplayNameChange(const std(const std::string& json);
std::string string& newName);(const std::string& json);
std::string buildAvatarUrlChange(const std(const std::string& json);
std::string string& mxcUrl);(const std::string& json);
std::string formatProfileSummary(const UserProfile& profile);(const std::string& json);
