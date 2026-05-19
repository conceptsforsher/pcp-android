#include "progressive/signout_service.hpp"

namespace progressive {

// ==== Error classification for sign out ====
//
// Original Kotlin (SignOutTask.kt:53-58):
//   if (throwable is Failure.ServerError &&
//       throwable.httpCode == HTTP_UNAUTHORIZED && // 401
//       throwable.error.code == MatrixError.M_UNKNOWN_TOKEN)
//       // Ignore

bool shouldIgnoreSignOutError(const std::string& errorCode, int httpCode) {
    // Original Kotlin: M_UNKNOWN_TOKEN and 401 → ignore (token already invalid)
    return errorCode == "M_UNKNOWN_TOKEN" && httpCode == 401;
}

// ==== Sign Out Logic ====
//
// Original Kotlin (DefaultSignOutTask.kt:42-80):
//   cleanupSession.stopActiveTasks()
//   executeRequest { signOutAPI.signOut() }
//   catch { if M_UNKNOWN_TOKEN, ignore }
//   identityDisconnectTask.execute(Unit)
//   cleanupSession.cleanup()

SignOutResult processSignOut(const SignOutParams& params) {
    SignOutResult result;

    // Original Kotlin: sign out from homeserver if requested
    if (params.signOutFromHomeserver) {
        result.signOutSent = true;
        // Note: actual HTTP call happens at Kotlin layer via ProgressiveNative.
        // Here we just track the intent. The Kotlin fallback handles the actual
        // POST /_matrix/client/r0/logout call.

        // If the server returns M_UNKNOWN_TOKEN (401), we ignore it
        // because the token may have already been invalidated.
        if (params.ignoreServerRequestError) {
            result.success = true;
        }
    }

    // Original Kotlin: identityDisconnectTask.execute(Unit)
    // C++ just tracks the state; Kotlin handles the actual disconnect.
    result.identityDisconnected = true;

    // Original Kotlin: cleanupSession.cleanup()
    result.sessionCleaned = true;

    result.success = true;
    return result;
}

// ==== JSON Helpers for Sign In Again ====
//
// Original Kotlin (DefaultSignInAgainTask.kt:35-48):
//   PasswordLoginParams.userIdentifier(
//       user = sessionParams.userId,
//       password = params.password,
//       deviceDisplayName = null,
//       deviceId = sessionParams.deviceId
//   )
//
// JSON body for POST /_matrix/client/r0/login:
//   {
//     "type": "m.login.password",
//     "identifier": {"type": "m.id.user", "user": "@alice:example.org"},
//     "password": "secret",
//     "device_id": "ABCDEFGH"
//   }

std::string signInAgainBodyToJson(const SignInAgainParams& params) {
    std::string json = "{";
    // Original Kotlin: type = m.login.password
    json += "\"type\":\"m.login.password\",";
    // Original Kotlin: identifier block
    json += "\"identifier\":{\"type\":\"m.id.user\",\"user\":\"" + params.userId + "\"},";
    // Original Kotlin: password
    json += "\"password\":\"" + params.password + "\",";
    // Original Kotlin: deviceId (same device, re-login)
    if (!params.deviceId.empty()) {
        json += "\"device_id\":\"" + params.deviceId + "\",";
    }
    // Remove trailing comma
    if (json.back() == ',') json.pop_back();
    json += "}";
    return json;
}

// Parse login response: {"access_token":"syt_...","device_id":"...","user_id":"..."}
//
// Original Kotlin: result is Credentials object
SignInAgainResult parseLoginResponse(const std::string& json, const std::string& deviceId) {
    SignInAgainResult result;

    // Extract access_token
    auto pos = json.find("\"access_token\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
            if (pos < json.size() && json[pos] == '"') {
                pos++;
                size_t end = pos;
                while (end < json.size() && json[end] != '"') end++;
                result.accessToken = json.substr(pos, end - pos);
            }
        }
    }

    // Extract device_id
    pos = json.find("\"device_id\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
            if (pos < json.size() && json[pos] == '"') {
                pos++;
                size_t end = pos;
                while (end < json.size() && json[end] != '"') end++;
                result.deviceId = json.substr(pos, end - pos);
            }
        }
    }

    // Extract user_id
    pos = json.find("\"user_id\"");
    if (pos != std::string::npos) {
        pos = json.find(':', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
            if (pos < json.size() && json[pos] == '"') {
                pos++;
                size_t end = pos;
                while (end < json.size() && json[end] != '"') end++;
                result.userId = json.substr(pos, end - pos);
            }
        }
    }

    result.deviceId = deviceId; // keep original deviceId if not in response
    result.success = !result.accessToken.empty() && !result.userId.empty();

    return result;
}

} // namespace progressive
