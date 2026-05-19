#include "progressive/openid_token.hpp"
#include <cstring>
#include <chrono>

namespace progressive {

// ============================================================================
// Manual JSON helpers
// ============================================================================

static std::string jsonEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return 0;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

static int extractInt(const std::string& json, const std::string& key) {
    return static_cast<int>(extractInt64(json, key));
}

static bool extractBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

// ============================================================================
// OpenIdToken (legacy from original)
// ============================================================================
//
// Original Kotlin (OpenIdToken.kt:24-51):
//   data class OpenIdToken(...)

OpenIdToken parseOpenIdToken(const std::string& json) {
    OpenIdToken token;
    token.accessToken = extractStr(json, "access_token");
    token.tokenType = extractStr(json, "token_type");
    token.matrixServerName = extractStr(json, "matrix_server_name");
    token.expiresIn = extractInt(json, "expires_in");
    return token;
}

std::string openIdTokenToJson(const OpenIdToken& token) {
    std::string json = "{";
    json += "\"access_token\":\"" + jsonEscape(token.accessToken) + "\",";
    json += "\"token_type\":\"" + jsonEscape(token.tokenType) + "\",";
    json += "\"matrix_server_name\":\"" + jsonEscape(token.matrixServerName) + "\",";
    json += "\"expires_in\":" + std::to_string(token.expiresIn);
    json += "}";
    return json;
}

// ============================================================================
// OpenIdCredentials
// ============================================================================
//
// Original Kotlin (OpenIdCredentials.kt):
//   data class OpenIdCredentials(...)

bool OpenIdCredentials::isExpired() const {
    if (expiresIn <= 0 || issuedAtMs <= 0) return true;

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    int64_t expiresAtMs = issuedAtMs + (expiresIn * 1000);
    return now >= expiresAtMs;
}

bool OpenIdCredentials::isValid() const {
    return !accessToken.empty() && tokenType == "Bearer"
        && !matrixServerName.empty() && expiresIn > 0 && !isExpired();
}

// ============================================================================
// OpenIdTokenResponse
// ============================================================================
//
// Original Kotlin (RequestOpenIdTokenResponse.kt:23-48):
//   @JsonClass(generateAdapter = true)
//   internal data class RequestOpenIdTokenResponse(...)

OpenIdToken OpenIdTokenResponse::toToken() const {
    OpenIdToken token;
    token.accessToken = accessToken;
    token.tokenType = tokenType;
    token.matrixServerName = matrixServerName;
    token.expiresIn = static_cast<int>(expiresIn);
    return token;
}

OpenIdCredentials OpenIdTokenResponse::toCredentials() const {
    OpenIdCredentials creds;
    creds.accessToken = accessToken;
    creds.tokenType = tokenType;
    creds.expiresIn = expiresIn;
    creds.matrixServerName = matrixServerName;
    creds.issuedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    return creds;
}

bool OpenIdTokenResponse::isValid() const {
    return !accessToken.empty() && tokenType == "Bearer"
        && !matrixServerName.empty() && expiresIn > 0;
}

// ============================================================================
// OpenID Token Request
// ============================================================================
//
// Original Kotlin (OpenIdAPI.kt:37-41):
//   @POST("user/{userId}/openid/request_token")
//   suspend fun openIdToken(
//       @Path("userId") userId: String,
//       @Body body: JsonDict = emptyMap()
//   ): OpenIdToken

std::string buildOpenIdTokenRequest(const std::string& userId) {
    // Original Kotlin: POST body is an empty JSON dict {}
    (void)userId; // userId is used in the URL path, not the body
    return "{}";
}

std::string buildOpenIdTokenEndpoint(const std::string& homeserverBase,
                                      const std::string& userId) {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/client/r0/user/" + userId + "/openid/request_token";
}

OpenIdTokenResponse parseOpenIdTokenResponse(const std::string& responseJson) {
    OpenIdTokenResponse response;
    response.accessToken = extractStr(responseJson, "access_token");
    response.tokenType = extractStr(responseJson, "token_type");
    response.matrixServerName = extractStr(responseJson, "matrix_server_name");
    response.expiresIn = extractInt64(responseJson, "expires_in");
    return response;
}

std::string openIdTokenResponseToJson(const OpenIdTokenResponse& response) {
    std::string json = "{";
    json += "\"access_token\":\"" + jsonEscape(response.accessToken) + "\",";
    json += "\"token_type\":\"" + jsonEscape(response.tokenType) + "\",";
    json += "\"matrix_server_name\":\"" + jsonEscape(response.matrixServerName) + "\",";
    json += "\"expires_in\":" + std::to_string(response.expiresIn);
    json += "}";
    return json;
}

// ============================================================================
// isTokenExpired
// ============================================================================
//
// Original Kotlin (OpenIdToken.kt utility):
//   fun isTokenExpired(token: OpenIdToken, issuedAtMs: Long): Boolean

bool isTokenExpired(const OpenIdCredentials& credentials) {
    return credentials.isExpired();
}

bool isTokenExpired(const OpenIdToken& token, int64_t issuedAtMs) {
    if (token.expiresIn <= 0 || issuedAtMs <= 0) return true;

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    int64_t expiresAtMs = issuedAtMs + (token.expiresIn * 1000);
    return now >= expiresAtMs;
}

// ============================================================================
// Third-Party Lookup
// ============================================================================
//
// Original Kotlin (IdentityAPI.kt / ThirdPartyLookup.kt):
//   POST /_matrix/identity/v2/lookup

std::string buildThirdPartyLookupRequest(const ThirdPartyLookupRequest& request) {
    std::string json = "{";
    json += "\"address\":\"" + jsonEscape(request.address) + "\",";
    json += "\"medium\":\"" + jsonEscape(request.medium) + "\",";
    json += "\"id_access_token\":\"" + jsonEscape(request.idAccessToken) + "\"";
    if (!request.idServer.empty()) {
        json += ",\"id_server\":\"" + jsonEscape(request.idServer) + "\"";
    }
    json += "}";
    return json;
}

std::string buildThirdPartyLookupEndpoint(const std::string& idServer) {
    std::string base = idServer;
    if (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/identity/v2/lookup";
}

ThirdPartyLookupResponse parseThirdPartyLookupResponse(const std::string& responseJson) {
    ThirdPartyLookupResponse response;

    // Original Kotlin (LookupResponse.kt):
    //   The response contains "address", "medium", "mxid", "display_name", "avatar_url"
    response.mxid = extractStr(responseJson, "mxid");
    response.displayName = extractStr(responseJson, "display_name");
    response.avatarUrl = extractStr(responseJson, "avatar_url");

    // Check if a user was found
    response.found = !response.mxid.empty();
    response.success = response.found || responseJson.empty();

    return response;
}

std::string thirdPartyLookupResponseToJson(const ThirdPartyLookupResponse& response) {
    std::string json = "{";
    json += "\"found\":" + std::string(response.found ? "true" : "false") + ",";
    json += "\"mxid\":\"" + jsonEscape(response.mxid) + "\",";
    json += "\"displayName\":\"" + jsonEscape(response.displayName) + "\",";
    json += "\"avatarUrl\":\"" + jsonEscape(response.avatarUrl) + "\",";
    json += "\"success\":" + std::string(response.success ? "true" : "false");
    json += "}";
    return json;
}

// ============================================================================
// Widget OpenID
// ============================================================================
//
// Original Kotlin (WidgetOpenId.kt / WidgetAPI.kt):
//   data class WidgetOpenIdRequest(val widgetId: String)
//   data class WidgetOpenIdResponse(...)

std::string buildWidgetOpenIdRequest(const WidgetOpenIdRequest& request) {
    std::string json = "{";
    json += "\"widgetId\":\"" + jsonEscape(request.widgetId) + "\",";
    json += "\"roomId\":\"" + jsonEscape(request.roomId) + "\",";
    json += "\"userId\":\"" + jsonEscape(request.userId) + "\"";
    if (!request.clientSecret.empty()) {
        json += ",\"clientSecret\":\"" + jsonEscape(request.clientSecret) + "\"";
    }
    if (!request.redirectUri.empty()) {
        json += ",\"redirectUri\":\"" + jsonEscape(request.redirectUri) + "\"";
    }
    if (!request.state.empty()) {
        json += ",\"state\":\"" + jsonEscape(request.state) + "\"";
    }
    json += "}";
    return json;
}

WidgetOpenIdResponse parseWidgetOpenIdResponse(const std::string& responseJson) {
    WidgetOpenIdResponse response;
    response.accessToken = extractStr(responseJson, "access_token");
    response.tokenType = extractStr(responseJson, "token_type");
    response.matrixServerName = extractStr(responseJson, "matrix_server_name");
    response.expiresIn = extractInt64(responseJson, "expires_in");
    response.widgetId = extractStr(responseJson, "widgetId");
    return response;
}

OpenIdToken WidgetOpenIdResponse::toOpenIdToken() const {
    OpenIdToken token;
    token.accessToken = accessToken;
    token.tokenType = tokenType;
    token.matrixServerName = matrixServerName;
    token.expiresIn = static_cast<int>(expiresIn);
    return token;
}

OpenIdCredentials WidgetOpenIdResponse::toCredentials() const {
    OpenIdCredentials creds;
    creds.accessToken = accessToken;
    creds.tokenType = tokenType;
    creds.expiresIn = expiresIn;
    creds.matrixServerName = matrixServerName;
    creds.issuedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    return creds;
}

std::string widgetOpenIdResponseToJson(const WidgetOpenIdResponse& response) {
    std::string json = "{";
    json += "\"access_token\":\"" + jsonEscape(response.accessToken) + "\",";
    json += "\"token_type\":\"" + jsonEscape(response.tokenType) + "\",";
    json += "\"matrix_server_name\":\"" + jsonEscape(response.matrixServerName) + "\",";
    json += "\"expires_in\":" + std::to_string(response.expiresIn) + ",";
    json += "\"widgetId\":\"" + jsonEscape(response.widgetId) + "\"";
    json += "}";
    return json;
}

// ============================================================================
// OpenID Callback / Userinfo URLs
// ============================================================================

std::string buildWidgetOpenIdCallback(const std::string& homeserverBase,
                                       const std::string& widgetId,
                                       const std::string& userId) {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/client/r0/user/" + userId
           + "/openid/request_token?widgetId=" + widgetId;
}

std::string buildOpenIdUserinfoUrl(const std::string& homeserverBase,
                                    const std::string& userId,
                                    const std::string& accessToken) {
    std::string base = homeserverBase;
    if (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/client/r0/user/" + userId
           + "/openid/userinfo?access_token=" + accessToken;
}

} // namespace progressive
