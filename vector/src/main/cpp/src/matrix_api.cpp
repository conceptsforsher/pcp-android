#include "progressive/matrix_api.hpp"
#include "progressive/tls_bridge.hpp"
#include "progressive/string_utils.hpp"
#include <sstream>

namespace progressive {

// ==== Global State ====

static std::string g_homeserverBase;
static std::string g_accessToken;

void setHomeserverBaseUrl(const std::string& url) {
    g_homeserverBase = url;
    if (!g_homeserverBase.empty() && g_homeserverBase.back() == '/')
        g_homeserverBase.pop_back();
}

const std::string& getHomeserverBaseUrl() { return g_homeserverBase; }

void setAccessToken(const std::string& token) { g_accessToken = token; }

bool nativeApiAvailable() {
    return tlsBridgeAvailable() && !g_homeserverBase.empty();
}

// ==== Helper: make authenticated GET ====

static HttpResponse authGet(const std::string& path, int timeout = 30000) {
    std::string url = g_homeserverBase + path;
    std::unordered_map<std::string, std::string> headers;
    if (!g_accessToken.empty()) headers["Authorization"] = "Bearer " + g_accessToken;
    return httpGet(url, headers, timeout);
}

// Helper: make authenticated POST

static HttpResponse authPost(const std::string& path, const std::string& jsonBody, int timeout = 30000) {
    std::string url = g_homeserverBase + path;
    std::unordered_map<std::string, std::string> headers;
    if (!g_accessToken.empty()) headers["Authorization"] = "Bearer " + g_accessToken;
    return httpPost(url, jsonBody, headers, timeout);
}

// Helper: make authenticated PUT

static HttpResponse authPut(const std::string& path, const std::string& jsonBody, int timeout = 30000) {
    std::string url = g_homeserverBase + path;
    std::unordered_map<std::string, std::string> headers;
    if (!g_accessToken.empty()) headers["Authorization"] = "Bearer " + g_accessToken;
    return httpPut(url, jsonBody, timeout);
}

// ==== Auth API ====

std::string apiGetVersions() {
    auto resp = httpGet(g_homeserverBase + "/_matrix/client/versions", {}, 10000);
    return resp.success ? resp.body : "";
}

std::string apiGetLoginFlows() {
    auto resp = httpGet(g_homeserverBase + "/_matrix/client/r0/login", {}, 10000);
    return resp.success ? resp.body : "";
}

Credentials apiLogin(const std::string& userId, const std::string& password,
                     const std::string& deviceId) {
    // Build login JSON body
    std::ostringstream body;
    body << R"({"type":"m.login.password","identifier":{"type":"m.id.user","user":")"
         << userId << R"("},"password":")" << password << R"(")";
    if (!deviceId.empty()) body << R"(,"device_id":")" << deviceId << R"(")";
    body << "}";

    auto resp = httpPost(g_homeserverBase + "/_matrix/client/r0/login", body.str(),
        {{"Content-Type", "application/json"}}, 15000);

    if (!resp.isOk()) return {};

    return parseCredentials(resp.body);
}

std::string apiRegister(const std::string& username, const std::string& password,
                        const std::string& deviceId) {
    std::ostringstream body;
    body << R"({"username":")" << username << R"(","password":")" << password << R"(")";
    if (!deviceId.empty()) body << R"(,"device_id":")" << deviceId << R"(")";
    body << R"(,"initial_device_display_name":"Progressive Chat")";
    body << "}";

    auto resp = httpPost(g_homeserverBase + "/_matrix/client/r0/register",
        body.str(), {{"Content-Type", "application/json"}}, 15000);
    return resp.success ? resp.body : "";
}

bool apiUsernameAvailable(const std::string& username) {
    auto resp = httpGet(g_homeserverBase + "/_matrix/client/r0/register/available?username=" + username);
    if (!resp.isOk()) return false;
    // Response: {"available": true/false}
    return resp.body.find("\"available\":true") != std::string::npos ||
           resp.body.find("\"available\": true") != std::string::npos;
}

// ==== Sync API ====

SyncResponse apiSync(const std::string& filter, const std::string& since, int timeout) {
    std::string path = "/_matrix/client/r0/sync?timeout=" + std::to_string(timeout);
    if (!filter.empty()) path += "&filter=" + filter;
    if (!since.empty()) path += "&since=" + since;

    auto resp = authGet(path, timeout + 5000);
    if (!resp.isOk()) return {};

    return parseSyncResponse(resp.body);
}

// ==== Room API ====

std::string apiCreateRoom(const std::string& name, const std::string& topic,
                          bool isDirect, const std::vector<std::string>& inviteUsers) {
    std::ostringstream body;
    body << "{";
    if (!name.empty()) body << R"("name":")" << name << R"(",)";
    if (!topic.empty()) body << R"("topic":")" << topic << R"(",)";
    if (isDirect) body << R"("is_direct":true,)";
    if (!inviteUsers.empty()) {
        body << R"("invite":[)";
        for (size_t i = 0; i < inviteUsers.size(); i++) {
            if (i > 0) body << ",";
            body << R"(")" << inviteUsers[i] << R"(")";
        }
        body << "],";
    }
    body << R"("preset":"private_chat")";
    body << "}";

    auto resp = authPost("/_matrix/client/r0/createRoom", body.str());
    return resp.success ? resp.body : "";
}

std::string apiGetRoomMessages(const std::string& roomId, const std::string& from,
                               const std::string& dir, int limit) {
    std::string path = "/_matrix/client/r0/rooms/" + roomId + "/messages?dir=" + dir
                     + "&limit=" + std::to_string(limit);
    if (!from.empty()) path += "&from=" + from;

    auto resp = authGet(path);
    return resp.success ? resp.body : "";
}

std::string apiSendEvent(const std::string& roomId, const std::string& eventType,
                         const std::string& txnId, const std::string& contentJson) {
    std::string path = "/_matrix/client/r0/rooms/" + roomId
                     + "/send/" + eventType + "/" + txnId;
    auto resp = authPut(path, contentJson);
    return resp.success ? resp.body : "";
}

std::string apiJoinRoom(const std::string& roomId, const std::string& reason) {
    std::string body = reason.empty() ? "{}" : R"({"reason":")" + reason + R"("})";
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/join", body);
    return resp.success ? resp.body : "";
}

std::string apiLeaveRoom(const std::string& roomId) {
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/leave", "{}");
    return resp.success ? resp.body : "";
}

// ==== Profile API ====

std::string apiGetProfile(const std::string& userId) {
    auto resp = authGet("/_matrix/client/r0/profile/" + userId);
    return resp.success ? resp.body : "";
}

std::string apiGetDisplayName(const std::string& userId) {
    auto resp = authGet("/_matrix/client/r0/profile/" + userId + "/displayname");
    return resp.success ? resp.body : "";
}

std::string apiSetDisplayName(const std::string& userId, const std::string& displayName) {
    std::string body = R"({"displayname":")" + displayName + R"("})";
    auto resp = authPut("/_matrix/client/r0/profile/" + userId + "/displayname", body);
    return resp.success ? resp.body : "";
}

// ==== Media API ====

std::string apiUploadMedia(const std::string& fileName, const std::string& contentType,
                           const std::vector<uint8_t>& /*data*/) {
    // Media upload requires multipart form-data which is more complex.
    // For now, this is a stub — use Retrofit for media uploads.
    // Full implementation would build a multipart body with raw bytes.
    std::string path = "/_matrix/media/r0/upload?filename=" + fileName;
    auto resp = authPost(path, "{}"); // simplified
    return resp.success ? resp.body : "";
}

// ==== Additional API Implementations ====

std::string apiWhoAmI() {
    auto resp = authGet("/_matrix/client/r0/account/whoami");
    return resp.success ? resp.body : "";
}

bool apiLogout() {
    auto resp = authPost("/_matrix/client/r0/logout", "{}");
    return resp.isOk();
}

bool apiLogoutAll() {
    auto resp = authPost("/_matrix/client/r0/logout/all", "{}");
    return resp.isOk();
}

std::string apiGetPushRules() {
    auto resp = authGet("/_matrix/client/r0/pushrules");
    return resp.success ? resp.body : "";
}

std::string apiGetPushRule(const std::string& scope, const std::string& kind,
                           const std::string& ruleId) {
    std::string path = "/_matrix/client/r0/pushrules/" + scope + "/" + kind + "/" + ruleId;
    auto resp = authGet(path);
    return resp.success ? resp.body : "";
}

std::string apiSetPushRule(const std::string& scope, const std::string& kind,
                           const std::string& ruleId, const std::string& body) {
    std::string path = "/_matrix/client/r0/pushrules/" + scope + "/" + kind + "/" + ruleId;
    auto resp = authPut(path, body);
    return resp.success ? resp.body : "";
}

bool apiDeletePushRule(const std::string& scope, const std::string& kind,
                       const std::string& ruleId) {
    std::string path = "/_matrix/client/r0/pushrules/" + scope + "/" + kind + "/" + ruleId;
    auto resp = httpExecute({"DELETE", g_homeserverBase + path, "",
        {{"Authorization", "Bearer " + g_accessToken}}});
    return resp.isOk();
}

std::string apiCreateFilter(const std::string& userId, const std::string& filterJson) {
    auto resp = authPost("/_matrix/client/r0/user/" + userId + "/filter", filterJson);
    return resp.success ? resp.body : "";
}

std::string apiPublicRooms(const std::string& server, const std::string& searchTerm, int limit) {
    std::ostringstream body;
    body << R"({"limit":)" << limit;
    if (!server.empty()) body << R"(,"server":")" << server << R"(")";
    if (!searchTerm.empty()) body << R"(,"filter":{"generic_search_term":")" << searchTerm << R"("})";
    body << "}";
    auto resp = authPost("/_matrix/client/r0/publicRooms", body.str());
    return resp.success ? resp.body : "";
}

std::string apiSearch(const std::string& searchTerm, const std::string& roomId, int limit) {
    std::ostringstream body;
    body << R"({"search_categories":{"room_events":{"search_term":")" << searchTerm << R"(")";
    if (!roomId.empty()) body << R"(,"filter":{"rooms":[")" << roomId << R"("]})";
    body << R"(,"order_by":"recent","include_state":false)";
    body << "}}}";
    auto resp = authPost("/_matrix/client/r0/search", body.str());
    return resp.success ? resp.body : "";
}

std::string apiGetRoomMembers(const std::string& roomId) {
    auto resp = authGet("/_matrix/client/r0/rooms/" + roomId + "/members");
    return resp.success ? resp.body : "";
}

std::string apiInviteUser(const std::string& roomId, const std::string& userId,
                          const std::string& reason) {
    std::ostringstream body;
    body << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) body << R"(,"reason":")" << reason << R"(")";
    body << "}";
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/invite", body.str());
    return resp.success ? resp.body : "";
}

std::string apiKickUser(const std::string& roomId, const std::string& userId,
                        const std::string& reason) {
    std::ostringstream body;
    body << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) body << R"(,"reason":")" << reason << R"(")";
    body << "}";
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/kick", body.str());
    return resp.success ? resp.body : "";
}

std::string apiBanUser(const std::string& roomId, const std::string& userId,
                       const std::string& reason) {
    std::ostringstream body;
    body << R"({"user_id":")" << userId << R"(")";
    if (!reason.empty()) body << R"(,"reason":")" << reason << R"(")";
    body << "}";
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/ban", body.str());
    return resp.success ? resp.body : "";
}

std::string apiUnbanUser(const std::string& roomId, const std::string& userId) {
    std::ostringstream body;
    body << R"({"user_id":")" << userId << R"("})";
    auto resp = authPost("/_matrix/client/r0/rooms/" + roomId + "/unban", body.str());
    return resp.success ? resp.body : "";
}

std::string apiRedactEvent(const std::string& roomId, const std::string& eventId,
                           const std::string& txnId, const std::string& reason) {
    std::string path = "/_matrix/client/r0/rooms/" + roomId + "/redact/" + eventId + "/" + txnId;
    std::string body = reason.empty() ? "{}" : R"({"reason":")" + reason + R"("})";
    auto resp = authPut(path, body);
    return resp.success ? resp.body : "";
}

// Original Kotlin: resolveEndpointPath — fill {roomId}/{userId}/etc. placeholders
std::string resolveEndpointPath(const std::string& templ, const std::unordered_map<std::string, std::string>& vars) {
    std::string result = templ;
    for (const auto& [key, val] : vars) {
        std::string placeholder = "{" + key + "}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.size(), val);
            pos += val.size();
        }
    }
    return result;
}

// Original Kotlin: resolveRoomEndpoint — fill roomId into a path template
std::string resolveRoomEndpoint(const std::string& templ, const std::string& roomId) {
    return resolveEndpointPath(templ, {{"roomId", roomId}});
}

// Original Kotlin: resolveUserEndpoint — fill userId into a path template
std::string resolveUserEndpoint(const std::string& templ, const std::string& userId) {
    return resolveEndpointPath(templ, {{"userId", userId}});
}

// Original Kotlin: resolveEventEndpoint — fill roomId and eventId into path
std::string resolveEventEndpoint(const std::string& templ, const std::string& roomId, const std::string& eventId) {
    return resolveEndpointPath(templ, {{"roomId", roomId}, {"eventId", eventId}});
}

// Original Kotlin: apiGetRoomState — GET /_matrix/client/r0/rooms/{roomId}/state
std::string apiGetRoomState(const std::string& roomId) {
    return authGet(resolveRoomEndpoint(MatrixEndpointPath::ROOM_STATE, roomId)).body;
}

// Original Kotlin: apiGetRoomEvent — GET /_matrix/client/r0/rooms/{roomId}/event/{eventId}
std::string apiGetRoomEvent(const std::string& roomId, const std::string& eventId) {
    return authGet(resolveEventEndpoint(MatrixEndpointPath::ROOM_EVENT, roomId, eventId)).body;
}

// Original Kotlin: apiGetRoomContext — GET /_matrix/client/r0/rooms/{roomId}/context/{eventId}
std::string apiGetRoomContext(const std::string& roomId, const std::string& eventId) {
    return authGet(resolveEventEndpoint(MatrixEndpointPath::ROOM_CONTEXT, roomId, eventId)).body;
}

// Original Kotlin: apiChangePassword — POST /_matrix/client/r0/account/password
std::string apiChangePassword(const std::string& oldPassword, const std::string& newPassword) {
    std::ostringstream body;
    body << R"({"new_password":")" << newPassword << R"(")";
    if (!oldPassword.empty()) body << R"(,"old_password":")" << oldPassword << R"(")";
    body << "}";
    auto resp = authPost(MatrixEndpointPath::CHANGE_PASSWORD, body.str());
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiDeactivateAccount — POST /_matrix/client/r0/account/deactivate
std::string apiDeactivateAccount(const std::string& authJson) {
    auto resp = authPost(MatrixEndpointPath::DEACTIVATE_ACCOUNT, authJson.empty() ? "{}" : authJson);
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiSetPresence — PUT /_matrix/client/r0/presence/{userId}/status
std::string apiSetPresence(const std::string& userId, const std::string& presence) {
    std::string path = resolveUserEndpoint(MatrixEndpointPath::PRESENCE, userId);
    std::string body = R"({"presence":")" + presence + R"("})";
    auto resp = authPut(path, body);
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiGetPresence — GET /_matrix/client/r0/presence/{userId}/status
std::string apiGetPresence(const std::string& userId) {
    return authGet(resolveUserEndpoint(MatrixEndpointPath::PRESENCE, userId)).body;
}

// Original Kotlin: apiSetAvatarUrl — PUT avatar URL
std::string apiSetAvatarUrl(const std::string& userId, const std::string& avatarUrl) {
    std::string path = resolveUserEndpoint(MatrixEndpointPath::AVATARURL, userId);
    std::string body = R"({"avatar_url":")" + avatarUrl + R"("})";
    auto resp = authPut(path, body);
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiGetAvatarUrl — GET avatar URL
std::string apiGetAvatarUrl(const std::string& userId) {
    return authGet(resolveUserEndpoint(MatrixEndpointPath::AVATARURL, userId)).body;
}

// Original Kotlin: apiGetCapabilities — GET /_matrix/client/r0/capabilities
std::string apiGetCapabilities() {
    return authGet(MatrixEndpointPath::CAPABILITIES).body;
}

// Original Kotlin: apiGetWellKnown — GET /.well-known/matrix/client
std::string apiGetWellKnown(const std::string& serverName) {
    std::string url = "https://" + serverName + MatrixEndpointPath::WELL_KNOWN;
    auto resp = httpGet(url, {}, 10000);
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiGetTurnServer — GET /_matrix/client/r0/voip/turnServer
std::string apiGetTurnServer() {
    return authGet(MatrixEndpointPath::TURN_SERVER).body;
}

// Original Kotlin: apiReportEvent — POST report
std::string apiReportEvent(const std::string& roomId, const std::string& eventId,
                           const std::string& reason, int score) {
    std::string path = resolveEventEndpoint(MatrixEndpointPath::REPORT, roomId, eventId);
    std::ostringstream body;
    body << R"({"reason":")" << reason << R"(","score":)" << score << "}";
    auto resp = authPost(path, body.str());
    return resp.success ? resp.body : "";
}

// Original Kotlin: detectBestApiVersion — probe server for best supported version
MatrixApiVersion detectBestApiVersion(const std::string& baseUrl) {
    auto resp = httpGet(baseUrl + "/_matrix/client/versions", {}, 10000);
    if (!resp.success) return MatrixApiVersion::V1;
    if (resp.body.find("v1.1") != std::string::npos || resp.body.find("r0.6") != std::string::npos) {
        return MatrixApiVersion::V3;
    }
    return MatrixApiVersion::V1;
}

// Original Kotlin: makeApiUrl — build full URL using MatrixEndpointPath constants
std::string makeApiUrl(const std::string& path, const std::unordered_map<std::string, std::string>& params) {
    return buildEndpointUrl(g_homeserverBase, path, params);
}

// Original Kotlin: apiGetUrlPreview — GET /_matrix/media/r0/preview_url
std::string apiGetUrlPreview(const std::string& url, int64_t ts) {
    std::string path = MatrixEndpointPath::PREVIEW_URL;
    std::ostringstream fullPath;
    fullPath << path << "?url=" << urlEncode(url);
    if (ts > 0) fullPath << "&ts=" << ts;
    return authGet(fullPath.str()).body;
}

// Original Kotlin: apiSearchUserDirectory — POST user_directory/search
std::string apiSearchUserDirectory(const std::string& searchTerm, int limit) {
    std::ostringstream body;
    body << R"({"search_term":")" << searchTerm << R"(","limit":)" << limit << "}";
    auto resp = authPost(MatrixEndpointPath::USER_DIRECTORY, body.str());
    return resp.success ? resp.body : "";
}

// Original Kotlin: apiGetNotifications — GET /_matrix/client/r0/notifications
std::string apiGetNotifications(const std::string& from, int limit, const std::string& only) {
    std::ostringstream path;
    path << MatrixEndpointPath::NOTIFICATIONS << "?limit=" << limit;
    if (!from.empty()) path << "&from=" << from;
    if (!only.empty()) path << "&only=" << only;
    return authGet(path.str()).body;
}

// Original Kotlin: apiGetDevices — GET /_matrix/client/r0/devices
std::string apiGetDevices() {
    return authGet(MatrixEndpointPath::DEVICES).body;
}

// Original Kotlin: apiDeleteDevices — POST delete_devices
std::string apiDeleteDevices(const std::vector<std::string>& deviceIds) {
    std::ostringstream body;
    body << R"({"devices":[)";
    for (size_t i = 0; i < deviceIds.size(); i++) {
        if (i > 0) body << ",";
        body << R"(")" << deviceIds[i] << R"(")";
    }
    body << "]}";
    auto resp = authPost(MatrixEndpointPath::DELETE_DEVICES, body.str());
    return resp.success ? resp.body : "";
}

} // namespace progressive
