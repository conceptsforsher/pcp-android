#pragma once

#include <string>
#include <unordered_map>
#include "progressive/http_client.hpp"
#include "progressive/sync_models.hpp"
#include "progressive/auth_models.hpp"

namespace progressive {

// ==== Native Matrix API Client ====
//
// C++ replacements for Retrofit API calls. Each function maps to a
// Matrix Client-Server API endpoint. Used when SETTINGS_LABS_NATIVE_HTTP is ON.
// Original Kotlin Retrofit code remains as fallback.
//
// All functions are synchronous (blocking). The caller is responsible for
// running them on a background thread.

// Base URL for the homeserver (set once during init).
void setHomeserverBaseUrl(const std::string& url);
const std::string& getHomeserverBaseUrl();
void setAccessToken(const std::string& token);

// ==== Auth API ====

// GET /_matrix/client/versions — server version support
// Returns raw JSON response body
std::string apiGetVersions();

// GET /_matrix/client/r0/login — get available login flows
std::string apiGetLoginFlows();

// POST /_matrix/client/r0/login — authenticate with password
// Body: {"type":"m.login.password","identifier":{"type":"m.id.user","user":"..."},"password":"..."}
Credentials apiLogin(const std::string& userId, const std::string& password,
                     const std::string& deviceId = "");

// POST /_matrix/client/r0/register — register a new account
std::string apiRegister(const std::string& username, const std::string& password,
                        const std::string& deviceId = "");

// GET /_matrix/client/r0/register/available — check if username is available
bool apiUsernameAvailable(const std::string& username);

// ==== Sync API ====

// GET /_matrix/client/r0/sync — full sync request
// filter: JSON filter string or empty
// since: pagination token or empty for initial sync
// timeout: long-poll timeout in ms (0 = no polling)
SyncResponse apiSync(const std::string& filter = "",
                     const std::string& since = "",
                     int timeout = 30000);

// ==== Room API ====

// POST /_matrix/client/r0/createRoom — create a new room
std::string apiCreateRoom(const std::string& name = "",
                          const std::string& topic = "",
                          bool isDirect = false,
                          const std::vector<std::string>& inviteUsers = {});

// GET /_matrix/client/r0/rooms/{roomId}/messages — get room messages
// from: pagination token (empty = latest)
// dir: "b" = backwards, "f" = forwards
// limit: max events to return
std::string apiGetRoomMessages(const std::string& roomId,
                               const std::string& from = "",
                               const std::string& dir = "b",
                               int limit = 20);

// PUT /_matrix/client/r0/rooms/{roomId}/send/{eventType}/{txnId} — send event
std::string apiSendEvent(const std::string& roomId,
                         const std::string& eventType,
                         const std::string& txnId,
                         const std::string& contentJson);

// POST /_matrix/client/r0/rooms/{roomId}/join — join room
std::string apiJoinRoom(const std::string& roomId, const std::string& reason = "");

// POST /_matrix/client/r0/rooms/{roomId}/leave — leave room
std::string apiLeaveRoom(const std::string& roomId);

// ==== Profile API ====

// GET /_matrix/client/r0/profile/{userId} — get user profile
std::string apiGetProfile(const std::string& userId);

// GET /_matrix/client/r0/profile/{userId}/displayname
std::string apiGetDisplayName(const std::string& userId);

// PUT /_matrix/client/r0/profile/{userId}/displayname
std::string apiSetDisplayName(const std::string& userId, const std::string& displayName);

// ==== Media API ====

// POST /_matrix/media/r0/upload — upload file
std::string apiUploadMedia(const std::string& fileName,
                           const std::string& contentType,
                           const std::vector<uint8_t>& data);

// ==== Utility ====

// Get the status of the native HTTP client.
// Returns true if the TLS bridge and homeserver URL are configured.
bool nativeApiAvailable();

// ==== Additional APIs ====

// GET /_matrix/client/r0/account/whoami — get current user info
std::string apiWhoAmI();

// POST /_matrix/client/r0/logout — invalidate access token
bool apiLogout();

// POST /_matrix/client/r0/logout/all — invalidate all access tokens
bool apiLogoutAll();

// GET /_matrix/client/r0/pushrules — get all push rules
std::string apiGetPushRules();

// GET /_matrix/client/r0/pushrules/{scope}/{kind}/{ruleId}
std::string apiGetPushRule(const std::string& scope, const std::string& kind,
                           const std::string& ruleId);

// PUT /_matrix/client/r0/pushrules/{scope}/{kind}/{ruleId} — set push rule
std::string apiSetPushRule(const std::string& scope, const std::string& kind,
                           const std::string& ruleId, const std::string& body);

// DELETE /_matrix/client/r0/pushrules/{scope}/{kind}/{ruleId}
bool apiDeletePushRule(const std::string& scope, const std::string& kind,
                       const std::string& ruleId);

// POST /_matrix/client/r0/user/{userId}/filter — create sync filter
std::string apiCreateFilter(const std::string& userId, const std::string& filterJson);

// POST /_matrix/client/r0/publicRooms — get public rooms
std::string apiPublicRooms(const std::string& server = "",
                           const std::string& searchTerm = "",
                           int limit = 20);

// POST /_matrix/client/r0/search — search messages
std::string apiSearch(const std::string& searchTerm,
                      const std::string& roomId = "",
                      int limit = 10);

// GET /_matrix/client/r0/rooms/{roomId}/members — get room members
std::string apiGetRoomMembers(const std::string& roomId);

// POST /_matrix/client/r0/rooms/{roomId}/invite — invite user
std::string apiInviteUser(const std::string& roomId, const std::string& userId,
                          const std::string& reason = "");

// POST /_matrix/client/r0/rooms/{roomId}/kick — kick user
std::string apiKickUser(const std::string& roomId, const std::string& userId,
                        const std::string& reason = "");

// POST /_matrix/client/r0/rooms/{roomId}/ban — ban user
std::string apiBanUser(const std::string& roomId, const std::string& userId,
                       const std::string& reason = "");

// POST /_matrix/client/r0/rooms/{roomId}/unban — unban user
std::string apiUnbanUser(const std::string& roomId, const std::string& userId);

// POST /_matrix/client/r0/rooms/{roomId}/redact/{eventId}/{txnId} — redact event
std::string apiRedactEvent(const std::string& roomId, const std::string& eventId,
                           const std::string& txnId, const std::string& reason = "");

// Original Kotlin: MatrixApiVersion — API version selector
enum class MatrixApiVersion {
    V1,
    V2,
    V3
};

// Original Kotlin: getApiPathPrefix — versioned path prefix
inline const char* getApiPathPrefix(MatrixApiVersion version) {
    switch (version) {
        case MatrixApiVersion::V1: return "/_matrix/client/r0";
        case MatrixApiVersion::V2: return "/_matrix/client/v1";
        case MatrixApiVersion::V3: return "/_matrix/client/v3";
    }
    return "/_matrix/client/r0";
}

// Original Kotlin: MatrixEndpointPath — all Matrix endpoint path constants
namespace MatrixEndpointPath {
    constexpr const char* LOGIN = "/_matrix/client/r0/login";
    constexpr const char* LOGOUT = "/_matrix/client/r0/logout";
    constexpr const char* REGISTER = "/_matrix/client/r0/register";
    constexpr const char* REFRESH_TOKEN = "/_matrix/client/r0/tokenrefresh";
    constexpr const char* CHANGE_PASSWORD = "/_matrix/client/r0/account/password";
    constexpr const char* DEACTIVATE_ACCOUNT = "/_matrix/client/r0/account/deactivate";
    constexpr const char* WHOAMI = "/_matrix/client/r0/account/whoami";
    constexpr const char* SYNC = "/_matrix/client/r0/sync";
    constexpr const char* FILTER = "/_matrix/client/r0/user/{userId}/filter";
    constexpr const char* CREATE_ROOM = "/_matrix/client/r0/createRoom";
    constexpr const char* JOIN_ROOM = "/_matrix/client/r0/rooms/{roomId}/join";
    constexpr const char* LEAVE_ROOM = "/_matrix/client/r0/rooms/{roomId}/leave";
    constexpr const char* INVITE = "/_matrix/client/r0/rooms/{roomId}/invite";
    constexpr const char* KICK = "/_matrix/client/r0/rooms/{roomId}/kick";
    constexpr const char* BAN = "/_matrix/client/r0/rooms/{roomId}/ban";
    constexpr const char* UNBAN = "/_matrix/client/r0/rooms/{roomId}/unban";
    constexpr const char* ROOM_STATE = "/_matrix/client/r0/rooms/{roomId}/state";
    constexpr const char* ROOM_MEMBERS = "/_matrix/client/r0/rooms/{roomId}/members";
    constexpr const char* ROOM_MESSAGES = "/_matrix/client/r0/rooms/{roomId}/messages";
    constexpr const char* ROOM_EVENT = "/_matrix/client/r0/rooms/{roomId}/event/{eventId}";
    constexpr const char* ROOM_CONTEXT = "/_matrix/client/r0/rooms/{roomId}/context/{eventId}";
    constexpr const char* ROOM_UPGRADE = "/_matrix/client/r0/rooms/{roomId}/upgrade";
    constexpr const char* SEND_EVENT = "/_matrix/client/r0/rooms/{roomId}/send/{eventType}/{txnId}";
    constexpr const char* REDACT_EVENT = "/_matrix/client/r0/rooms/{roomId}/redact/{eventId}/{txnId}";
    constexpr const char* UPLOAD = "/_matrix/media/r0/upload";
    constexpr const char* DOWNLOAD = "/_matrix/media/r0/download/{serverName}/{mediaId}";
    constexpr const char* THUMBNAIL = "/_matrix/media/r0/thumbnail/{serverName}/{mediaId}";
    constexpr const char* PREVIEW_URL = "/_matrix/media/r0/preview_url";
    constexpr const char* PRESENCE = "/_matrix/client/r0/presence/{userId}/status";
    constexpr const char* PROFILE = "/_matrix/client/r0/profile/{userId}";
    constexpr const char* DISPLAYNAME = "/_matrix/client/r0/profile/{userId}/displayname";
    constexpr const char* AVATARURL = "/_matrix/client/r0/profile/{userId}/avatar_url";
    constexpr const char* THREE_PID = "/_matrix/client/r0/account/3pid";
    constexpr const char* SEARCH = "/_matrix/client/r0/search";
    constexpr const char* USER_DIRECTORY = "/_matrix/client/r0/user_directory/search";
    constexpr const char* PUBLIC_ROOMS = "/_matrix/client/r0/publicRooms";
    constexpr const char* DEVICES = "/_matrix/client/r0/devices";
    constexpr const char* DELETE_DEVICES = "/_matrix/client/r0/delete_devices";
    constexpr const char* KEY_UPLOAD = "/_matrix/client/r0/keys/upload";
    constexpr const char* KEY_QUERY = "/_matrix/client/r0/keys/query";
    constexpr const char* CLAIM_KEYS = "/_matrix/client/r0/keys/claim";
    constexpr const char* ROOM_KEYS_VERSION = "/_matrix/client/r0/room_keys/version";
    constexpr const char* ROOM_KEYS = "/_matrix/client/r0/room_keys/keys";
    constexpr const char* PUSHERS = "/_matrix/client/r0/pushers";
    constexpr const char* PUSH_RULES = "/_matrix/client/r0/pushrules";
    constexpr const char* NOTIFICATIONS = "/_matrix/client/r0/notifications";
    constexpr const char* TAGS = "/_matrix/client/r0/user/{userId}/rooms/{roomId}/tags";
    constexpr const char* ACCOUNT_DATA = "/_matrix/client/r0/user/{userId}/account_data/{type}";
    constexpr const char* ROOM_ACCOUNT_DATA = "/_matrix/client/r0/user/{userId}/rooms/{roomId}/account_data/{type}";
    constexpr const char* TURN_SERVER = "/_matrix/client/r0/voip/turnServer";
    constexpr const char* CAPABILITIES = "/_matrix/client/r0/capabilities";
    constexpr const char* WELL_KNOWN = "/.well-known/matrix/client";
    constexpr const char* OPENID = "/_matrix/client/r0/user/{userId}/openid/request_token";
    constexpr const char* THIRD_PARTY_PROTOCOL = "/_matrix/client/r0/thirdparty/protocol/{protocol}";
    constexpr const char* ADMIN = "/_matrix/client/r0/admin";
    constexpr const char* REPORT = "/_matrix/client/r0/rooms/{roomId}/report/{eventId}";
    constexpr const char* CONTENT_SCAN = "/_matrix/client/r0/rooms/{roomId}/scan/{eventId}";
    constexpr const char* ROOM_KEYS_VERSION_SPECIFIC = "/_matrix/client/r0/room_keys/version/{version}";
    constexpr const char* ROOM_KEYS_SPECIFIC = "/_matrix/client/r0/room_keys/keys/{roomId}";
    constexpr const char* VOIP_TURN_SERVER = "/_matrix/client/r0/voip/turnServer";
}

// Original Kotlin: buildParamString — query string builder
inline std::string buildParamString(const std::unordered_map<std::string, std::string>& params) {
    std::string result;
    bool first = true;
    for (const auto& [key, value] : params) {
        if (!first) result += "&";
        result += key + "=" + value;
        first = false;
    }
    return result;
}

// Original Kotlin: buildEndpointUrl — full URL constructor
inline std::string buildEndpointUrl(const std::string& baseUrl, const std::string& path,
                                    const std::unordered_map<std::string, std::string>& params = {}) {
    std::string url = baseUrl;
    if (!url.empty() && url.back() == '/') url.pop_back();
    url += path;
    if (!params.empty()) {
        url += "?" + buildParamString(params);
    }
    return url;
}

// Original Kotlin: isStableEndpoint — check for stable API path
inline bool isStableEndpoint(const std::string& path) {
    return path.find("/unstable/") == std::string::npos;
}

// Original Kotlin: isUnstableEndpoint — check for unstable API path
inline bool isUnstableEndpoint(const std::string& path) {
    return path.find("/unstable/") != std::string::npos;
}

// Original Kotlin: getStableAlternative — /unstable/ -> /r0/ fallback
inline std::string getStableAlternative(const std::string& unstablePath) {
    std::string result = unstablePath;
    size_t pos = result.find("/unstable/");
    if (pos != std::string::npos) {
        result.replace(pos, 10, "/r0/");
    }
    return result;
}

// Original Kotlin: resolveEndpointPath — fill placeholder variables in template
std::string resolveEndpointPath(const std::string& templ, const std::unordered_map<std::string, std::string>& vars);

// Original Kotlin: resolveRoomEndpoint — fill roomId into a path template
std::string resolveRoomEndpoint(const std::string& templ, const std::string& roomId);

// Original Kotlin: resolveUserEndpoint — fill userId into a path template
std::string resolveUserEndpoint(const std::string& templ, const std::string& userId);

// Original Kotlin: resolveEventEndpoint — fill roomId and eventId into path
std::string resolveEventEndpoint(const std::string& templ, const std::string& roomId, const std::string& eventId);

// Original Kotlin: detectBestApiVersion — probe server for best supported version
MatrixApiVersion detectBestApiVersion(const std::string& baseUrl);

// Original Kotlin: additional API convenience wrappers
std::string apiGetRoomState(const std::string& roomId);
std::string apiGetRoomEvent(const std::string& roomId, const std::string& eventId);
std::string apiGetRoomContext(const std::string& roomId, const std::string& eventId);
std::string apiChangePassword(const std::string& oldPassword, const std::string& newPassword);
std::string apiDeactivateAccount(const std::string& authJson = "");
std::string apiSetPresence(const std::string& userId, const std::string& presence);
std::string apiGetPresence(const std::string& userId);
std::string apiSetAvatarUrl(const std::string& userId, const std::string& avatarUrl);
std::string apiGetAvatarUrl(const std::string& userId);
std::string apiGetCapabilities();
std::string apiGetWellKnown(const std::string& serverName);
std::string apiGetTurnServer();
std::string apiReportEvent(const std::string& roomId, const std::string& eventId,
                           const std::string& reason = "", int score = 0);

} // namespace progressive
