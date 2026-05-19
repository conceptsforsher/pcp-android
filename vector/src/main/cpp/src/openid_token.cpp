#include "progressive/openid_token.hpp"
#include <cstring>

namespace progressive {

// Manual JSON parsing for OpenID token response.
//
// Original Kotlin (GetOpenIdTokenTask.kt:30-35):
//   override suspend fun execute(params: Unit): OpenIdToken {
//       return executeRequest(globalErrorReceiver) {
//           openIdAPI.openIdToken(userId)
//       }
//   }
//
// The API endpoint POST /_matrix/client/r0/user/{userId}/openid/request_token
// expects an empty JSON body {} and returns:
//   {"access_token":"...","token_type":"Bearer","matrix_server_name":"...","expires_in":3600}

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

OpenIdToken parseOpenIdToken(const std::string& json) {
    OpenIdToken token;

    // Original Kotlin: val openIdToken.accessToken <=> response.accessToken
    token.accessToken = extractJsonString(json, "access_token");

    // Original Kotlin: tokenType is always "Bearer"
    token.tokenType = extractJsonString(json, "token_type");

    // Original Kotlin: server domain
    token.matrixServerName = extractJsonString(json, "matrix_server_name");

    // Original Kotlin: expires_in is an integer
    token.expiresIn = extractJsonInt(json, "expires_in");

    return token;
}

std::string openIdTokenToJson(const OpenIdToken& token) {
    // Original Kotlin: Moshi serialization of OpenIdToken
    std::string json = "{";
    json += "\"access_token\":\"" + token.accessToken + "\",";
    json += "\"token_type\":\"" + token.tokenType + "\",";
    json += "\"matrix_server_name\":\"" + token.matrixServerName + "\",";
    json += "\"expires_in\":" + std::to_string(token.expiresIn);
    json += "}";
    return json;
}

} // namespace progressive
