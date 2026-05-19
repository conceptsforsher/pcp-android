#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <cstdint>

namespace progressive {

// ==== User Account Data Types ====
//
// Original Kotlin (UserAccountDataTypes.kt:19-32):
//   object UserAccountDataTypes { const val TYPE_IGNORED_USER_LIST = "m.ignored_user_list", ... }

namespace UserAccountDataTypes {
    constexpr const char* TYPE_IGNORED_USER_LIST             = "m.ignored_user_list";
    constexpr const char* TYPE_DIRECT_MESSAGES               = "m.direct";
    constexpr const char* TYPE_BREADCRUMBS                   = "im.vector.setting.breadcrumbs";
    constexpr const char* TYPE_PREVIEW_URLS                  = "org.matrix.preview_urls";
    constexpr const char* TYPE_WIDGETS                       = "m.widgets";
    constexpr const char* TYPE_PUSH_RULES                    = "m.push_rules";
    constexpr const char* TYPE_INTEGRATION_PROVISIONING      = "im.vector.setting.integration_provisioning";
    constexpr const char* TYPE_ALLOWED_WIDGETS               = "im.vector.setting.allowed_widgets";
    constexpr const char* TYPE_IDENTITY_SERVER               = "m.identity_server";
    constexpr const char* TYPE_ACCEPTED_TERMS                = "m.accepted_terms";
    constexpr const char* TYPE_OVERRIDE_COLORS               = "im.vector.setting.override_colors";
    constexpr const char* TYPE_LOCAL_NOTIFICATION_SETTINGS_START = "org.matrix.msc3890.local_notification_settings.";
    constexpr const char* TYPE_USER_STATUS                  = "im.vector.user_status";
}

// Backward-compatible alias — kept for existing callers
namespace AccountDataType {
    constexpr const char* DIRECT_MESSAGES           = UserAccountDataTypes::TYPE_DIRECT_MESSAGES;
    constexpr const char* IGNORED_USER_LIST         = UserAccountDataTypes::TYPE_IGNORED_USER_LIST;
    constexpr const char* PUSH_RULES                = UserAccountDataTypes::TYPE_PUSH_RULES;
    constexpr const char* ACCEPTED_TERMS            = UserAccountDataTypes::TYPE_ACCEPTED_TERMS;
    constexpr const char* BREADCRUMBS               = UserAccountDataTypes::TYPE_BREADCRUMBS;
    constexpr const char* INTEGRATION_PROVISIONING  = UserAccountDataTypes::TYPE_INTEGRATION_PROVISIONING;
    constexpr const char* WIDGETS                   = "im.vector.modular.widgets"; // Original Kotlin: legacy widget type
    constexpr const char* USER_STATUS               = UserAccountDataTypes::TYPE_USER_STATUS;
}

// ==== Room Account Data Types ====
//
// Original Kotlin (RoomAccountDataTypes.kt:19-25):
//   object RoomAccountDataTypes { const val EVENT_TYPE_VIRTUAL_ROOM = "im.vector.is_virtual_room", ... }

namespace RoomAccountDataTypes {
    constexpr const char* EVENT_TYPE_VIRTUAL_ROOM  = "im.vector.is_virtual_room";
    constexpr const char* EVENT_TYPE_TAG           = "m.tag";
    constexpr const char* EVENT_TYPE_FULLY_READ    = "m.fully_read";
    constexpr const char* EVENT_TYPE_SPACE_ORDER   = "org.matrix.msc3230.space_order";
    constexpr const char* EVENT_TYPE_TAGGED_EVENTS = "m.tagged_events";
    constexpr const char* EVENT_TYPE_MARKER        = "m.marker";
}

// ==== Direct Message Map ====

// Original Kotlin (DirectMessagesContent.kt:22):
//   typealias DirectMessagesContent = Map<String, List<String>>
using DirectMessagesContent  = std::unordered_map<std::string, std::vector<std::string>>;
using DirectMessageMap       = DirectMessagesContent;

// Parse the m.direct account data event into a map.
// Format: {"@bob:server": ["!room1:server", "!room2:server"], ...}
// Returns: userId → list of DM room IDs
DirectMessageMap parseDirectMessageMap(const std::string& json);
std::string buildDirectMessageMapJson(const DirectMessageMap& map);

// Convenience aliases matching Kotlin content model names
inline DirectMessagesContent parseDirectMessagesContent(const std::string& json) {
    return parseDirectMessageMap(json);
}
inline std::string buildDirectMessagesContent(const DirectMessagesContent& content) {
    return buildDirectMessageMapJson(content);
}

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

// Original Kotlin (IgnoredUsersContent.kt:23-29):
//   data class IgnoredUsersContent(@Json(name="ignored_users") val ignoredUsers: Map<String, Any>)
// We use set<string> since values are empty objects in Matrix.

struct IgnoredUsersContent {
    std::set<std::string> ignoredUsers; // User IDs to ignore
};

// Parse the m.ignored_user_list account data event.
// Format: {"ignored_users": {"@spam:server": {}}}
std::vector<std::string> parseIgnoredUsers(const std::string& json);

// Build the ignored users JSON for updating account data.
std::string buildIgnoredUsersJson(const std::vector<std::string>& userIds);

// Convenience aliases matching Kotlin content model names
inline IgnoredUsersContent parseIgnoredUsersContent(const std::string& json) {
    auto vec = parseIgnoredUsers(json);
    IgnoredUsersContent content;
    content.ignoredUsers.insert(vec.begin(), vec.end());
    return content;
}
inline std::string buildIgnoredUsersContent(const IgnoredUsersContent& content) {
    return buildIgnoredUsersJson(
        std::vector<std::string>(content.ignoredUsers.begin(), content.ignoredUsers.end()));
}

// ==== Identity Server ====

// Original Kotlin (IdentityServerContent.kt:23-25):
//   data class IdentityServerContent(@Json(name="base_url") val baseUrl: String? = null)

struct IdentityServerContent {
    std::string baseUrl; // e.g. "https://matrix.org"
};

IdentityServerContent parseIdentityServerContent(const std::string& json);
std::string buildIdentityServerContent(const IdentityServerContent& content);

// ==== Accepted Terms ====

// Original Kotlin (AcceptedTermsContent.kt:23-25):
//   data class AcceptedTermsContent(@Json(name="accepted") val acceptedTerms: List<String> = emptyList())

struct AcceptedTermsContent {
    std::vector<std::string> acceptedTerms;
};

AcceptedTermsContent parseAcceptedTermsContent(const std::string& json);
std::string buildAcceptedTermsContent(const AcceptedTermsContent& content);

// ==== Breadcrumbs ====

// Original Kotlin (BreadcrumbsContent.kt:23-25):
//   data class BreadcrumbsContent(@Json(name="recent_rooms") val recentRoomIds: List<String> = emptyList())

struct BreadcrumbsContent {
    std::vector<std::string> recentRoomIds;
};

// Parse the im.vector.setting.breadcrumbs account data.
// Returns: ordered list of recently visited room IDs.
std::vector<std::string> parseBreadcrumbs(const std::string& json);

// Add a room to breadcrumbs (moves to front of list, max 20).
std::string addBreadcrumb(const std::string& currentJson, const std::string& roomId);

inline BreadcrumbsContent parseBreadcrumbsContent(const std::string& json) {
    BreadcrumbsContent content;
    content.recentRoomIds = parseBreadcrumbs(json);
    return content;
}
inline std::string buildBreadcrumbsContent(const BreadcrumbsContent& content) {
    // Reuse addBreadcrumb's JSON builder — content.recentRoomIds already ordered
    // We build from scratch to avoid the removal/insert/add logic
    std::string json;
    json.reserve(content.recentRoomIds.size() * 60 + 20);
    json = R"({"recent_rooms":[)";
    for (size_t i = 0; i < content.recentRoomIds.size(); ++i) {
        if (i > 0) json += ',';
        json += '"';
        json += content.recentRoomIds[i];
        json += '"';
    }
    json += "]}";
    return json;
}

// ==== Room Account Data ====

// Original Kotlin (RoomAccountDataEvent.kt:25-29):
//   data class RoomAccountDataEvent(val roomId: String, val type: String, val content: Content)

struct RoomAccountData {
    std::string roomId;
    std::string type;
    std::string content; // raw JSON content
};

RoomAccountData parseRoomAccountData(const std::string& json);
std::string buildRoomAccountData(const RoomAccountData& data);

// ==== Virtual Room ====

// Original Kotlin: The "im.vector.is_virtual_room" room account data marks a virtual room.
// Virtual rooms are local-only representations that map to a native room ID.

struct VirtualRoomInfo {
    std::string roomId;          // Matrix room ID (virtual)
    std::string nativeRoomId;    // Underlying native room ID
};

// Check if a room account data event indicates this is a virtual room.
inline bool isVirtualRoom(const RoomAccountData& data) {
    return data.type == RoomAccountDataTypes::EVENT_TYPE_VIRTUAL_ROOM;
}

// Check if a given room ID has a virtual room marker in the provided account data map.
// Typically used when iterating over room-level account data from /sync.
inline bool isVirtualRoom(const std::string& roomId,
                          const std::unordered_map<std::string, std::vector<RoomAccountData>>& roomAccountDataMap)
{
    auto it = roomAccountDataMap.find(roomId);
    if (it == roomAccountDataMap.end()) return false;
    for (const auto& ad : it->second) {
        if (ad.type == RoomAccountDataTypes::EVENT_TYPE_VIRTUAL_ROOM) return true;
    }
    return false;
}

// ==== Account Data Event Builders ====

// Original Kotlin (UserAccountDataEvent.kt:28-31):
//   data class UserAccountDataEvent(@Json(name="type") val type: String, @Json(name="content") val content: Content)
//
// Build a user account data event JSON object (type + content).
// This wraps the content for representing the event in JSON.
// PUT /_matrix/client/v3/user/{userId}/account_data/{type} body is just content[].

std::string buildUserAccountDataEvent(const std::string& type, const std::string& contentJson);

// Original Kotlin (RoomAccountDataEvent.kt:25-29):
//   data class RoomAccountDataEvent(val roomId: String, val type: String, val content: Content)
//
// Build a room account data event JSON (roomId + type + content).

std::string buildRoomAccountDataEvent(const std::string& roomId, const std::string& type,
                                      const std::string& contentJson);

// ==== User Profile (Account Data Level) ====

// Original Kotlin (User.kt:26-33):
//   data class User(val userId: String,
//       val displayName: String? = null, val avatarUrl: String? = null)

// NOTE: A more general MatrixUser is defined in event_models.hpp for sender info.
// This UserProfileContent is the account-data / profile-API specific model.

struct UserProfileContent {
    std::string displayName;
    std::string avatarUrl;
};

UserProfileContent parseUserProfileContent(const std::string& json);
std::string buildUserProfileContent(const UserProfileContent& content);

// ==== Presence ====

// Original Kotlin (PresenceEnum.kt:23-35):
//   enum class PresenceEnum(val value: String) { ONLINE("online"), OFFLINE("offline"),
//       UNAVAILABLE("unavailable"), BUSY("org.matrix.msc3026.busy") }

enum class PresenceEnum {
    OFFLINE = 0,
    ONLINE  = 1,
    UNAVAILABLE = 2,
    BUSY   = 3
};

PresenceEnum presenceEnumFromString(const std::string& s);
std::string presenceEnumToString(PresenceEnum p);

// Original Kotlin (UserPresence.kt:19-24):
//   data class UserPresence(val lastActiveAgo: Long? = null,
//       val statusMessage: String? = null, val isCurrentlyActive: Boolean? = null,
//       val presence: PresenceEnum = PresenceEnum.OFFLINE)

struct UserPresenceContent {
    int64_t  lastActiveAgo     = -1;       // -1 = absent
    std::string statusMessage;
    bool     isCurrentlyActive = false;
    PresenceEnum presence      = PresenceEnum::OFFLINE;
};

UserPresenceContent parseUserPresenceContent(const std::string& json);
std::string buildUserPresenceContent(const UserPresenceContent& content);

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
