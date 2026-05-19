#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== Account Data Utilities ====
//
// Matrix account data is per-user key-value storage synchronized
// across all user devices. Used for preferences, read markers,
// direct message lists, and ignored users.
//
// Type constants for known account data events.

namespace AccountDataType {
    constexpr const char* DIRECT_MESSAGES = "m.direct";
    constexpr const char* IGNORED_USER_LIST = "m.ignored_user_list";
    constexpr const char* PUSH_RULES = "m.push_rules";
    constexpr const char* ACCEPTED_TERMS = "m.accepted_terms";
    constexpr const char* BREADCRUMBS = "im.vector.setting.breadcrumbs";
    constexpr const char* INTEGRATION_PROVISIONING = "im.vector.setting.integration_provisioning";
    constexpr const char* WIDGETS = "im.vector.modular.widgets";
    constexpr const char* USER_STATUS = "im.vector.user_status";
}

// ==== Direct Message Map ====

// Parse the m.direct account data event into a map.
// Format: {"@bob:server": ["!room1:server", "!room2:server"], ...}
// Returns: userId → list of DM room IDs
using DirectMessageMap = std::unordered_map<std::string, std::vector<std::string>>;

DirectMessageMap parseDirectMessageMap(const std::string& json);
std::string buildDirectMessageMapJson(const DirectMessageMap& map);

// Check if a room is a direct message with a specific user.
inline bool isDirectMessageRoom(const DirectMessageMap& dmMap,
    const std::string& userId, const std::string& roomId)
{
    auto it = dmMap.find(userId);
    if (it == dmMap.end()) return false;
    for (const auto& rid : it->second) {
        if (rid == roomId) return true;
    }
    return false;
}

// Get the other participant in a DM room (not current user).
inline std::string getDmPartner(const DirectMessageMap& dmMap,
    const std::string& roomId, const std::string& currentUserId)
{
    for (const auto& [uid, rooms] : dmMap) {
        for (const auto& rid : rooms) {
            if (rid == roomId && uid != currentUserId) return uid;
        }
    }
    return "";
}

// ==== Ignored Users ====

// Parse the m.ignored_user_list account data event.
// Format: {"ignored_users": {"@spam:server": {}}}
std::vector<std::string> parseIgnoredUsers(const std::string& json);

// Build the ignored users JSON for updating account data.
std::string buildIgnoredUsersJson(const std::vector<std::string>& userIds);

// ==== Breadcrumbs ====

// Parse the im.vector.setting.breadcrumbs account data.
// Returns: ordered list of recently visited room IDs.
std::vector<std::string> parseBreadcrumbs(const std::string& json);

// Add a room to breadcrumbs (moves to front of list, max 20).
std::string addBreadcrumb(const std::string& currentJson, const std::string& roomId);

// ==== Session Helpers ====

// Device display name builder.
inline std::string buildDeviceDisplayName(const std::string& model = "Android",
    const std::string& appName = "Progressive Chat")
{
    return appName + " (" + model + ")";
}

// Validate a Matrix user ID format.
// Must be: @localpart:server
inline bool isValidUserId(const std::string& userId) {
    if (userId.empty() || userId[0] != '@') return false;
    auto colon = userId.find(':');
    return colon != std::string::npos && colon > 1 && colon < userId.size() - 1;
}

// Validate a Matrix room ID format.
// Must be: !opaque:server
inline bool isValidRoomId(const std::string& roomId) {
    if (roomId.empty() || roomId[0] != '!') return false;
    auto colon = roomId.find(':');
    return colon != std::string::npos && colon > 1 && colon < roomId.size() - 1;
}

// Extract server name from MXID.
// "@user:matrix.org" → "matrix.org"
inline std::string serverNameFromMxid(const std::string& mxid) {
    auto colon = mxid.find(':');
    if (colon == std::string::npos) return "";
    return mxid.substr(colon + 1);
}

// Extract localpart from MXID.
// "@user:matrix.org" → "user"
inline std::string localpartFromMxid(const std::string& mxid) {
    if (mxid.empty() || mxid[0] != '@') return "";
    auto colon = mxid.find(':');
    if (colon == std::string::npos) return "";
    return mxid.substr(1, colon - 1);
}

} // namespace progressive
