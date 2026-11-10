#pragma once
#include <string>
#include <cstdint>

std::string url;            // mxc(const std::string& json);
std::string // or https(const std::string& json);
std::string // URL(const std::string& json);
std::string thumbnailUrl;   // resized version(const std::string& json);
std::string initials;       // computed from display name (e.g. "AB")(const std::string& json);
std::string buildAvatarUrl(const std(const std::string& json);
std::string string& mxcUrl, const std(const std::string& json);
std::string string& homeserver);(const std::string& json);
std::string buildThumbnailUrl(const std(const std::string& json);
std::string string& mxcUrl, const std(const std::string& json);
std::string string& homeserver,(const std::string& json);
std::string int width, int height, const std(const std::string& json);
std::string string& method = "crop");(const std::string& json);
std::string AvatarInfo parseAvatarInfo(const std(const std::string& json);
std::string string& mxcUrl, const std(const std::string& json);
std::string string& displayName = "");(const std::string& json);
std::string computeInitials(const std(const std::string& json);
std::string string& displayName, const std(const std::string& json);
std::string string& userId = "");(const std::string& json);
std::string getAvatarColor(const std(const std::string& json);
std::string string& userId);(const std::string& json);
std::string formatAvatarSource(const AvatarInfo& info, const std(const std::string& json);
std::string string& homeserver);(const std::string& json);
