#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct UserProfile {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string statusMessage;
    std::string presence;       // "online", "offline", "unavailable"
    bool isActive = false;
    int64_t lastActiveAgoMs = 0;
};

// Parse user profile from /profile API response
UserProfile parseUserProfile(const std::string& json, const std::string& userId = "");

// Parse presence from m.presence event
UserProfile parsePresenceEvent(const std::string& userId, const std::string& json);

// Format display name with fallback to MXID localpart
std::string formatDisplayName(const std::string& displayName, const std::string& userId);

// Format user for mention in messages
std::string formatUserMention(const std::string& userId, const std::string& displayName);

// Format last active time ("Active now", "5m ago", "2h ago", "3d ago")
std::string formatLastActive(int64_t agoMs);

// Parse display name from member event
std::string extractDisplayName(const std::string& memberEventJson, const std::string& userId);

} // namespace pcp
