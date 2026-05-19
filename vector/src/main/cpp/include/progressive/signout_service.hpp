#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Sign Out — handles session termination, token invalidation,
// and credential renewal on the homeserver.
//
// Original Kotlin (SignOutService.kt:22-43):
//   interface SignOutService {
//       suspend fun signInAgain(password: String)
//       suspend fun updateCredentials(credentials: Credentials)
//       suspend fun signOut(signOutFromHomeserver: Boolean,
//           ignoreServerRequestError: Boolean = false)
//   }

struct SignOutResult {
    bool success = false;
    bool signOutSent = false;       // POST /logout was sent
    bool identityDisconnected = false; // identity server disconnected
    bool sessionCleaned = false;    // local data has been purged
    std::string errorMessage;       // error if any
    int errorHttpCode = 0;          // HTTP status if server error
    std::string errorMatrixCode;    // Matrix error code (e.g. M_UNKNOWN_TOKEN)
};

// Original Kotlin (SignOutTask.kt:34-39):
//   data class Params(
//       val signOutFromHomeserver: Boolean,
//       val ignoreServerRequestError: Boolean
//   )
struct SignOutParams {
    bool signOutFromHomeserver = true;
    bool ignoreServerRequestError = false;
};

// Original Kotlin (SignInAgainTask.kt:29-31):
//   data class Params(val password: String)
struct SignInAgainParams {
    std::string password;
    std::string userId;          // Same userId to reuse
    std::string deviceId;        // Same deviceId to reuse
    std::string homeServerUrl;   // Homeserver base URL
};

// Result of sign-in-again: new access token and credentials
struct SignInAgainResult {
    bool success = false;
    std::string accessToken;
    std::string deviceId;
    std::string userId;
    std::string homeServer;
    std::string errorMessage;
};

// Process sign out: determine if server request should be sent,
// whether to ignore errors, handle identity disconnect.
//
// Original Kotlin (DefaultSignOutTask.kt:42-80):
//   if (params.signOutFromHomeserver) { signOutAPI.signOut() } catch (M_UNKNOWN_TOKEN) ...
//   identityDisconnectTask.execute(Unit)
//   cleanupSession.cleanup()
SignOutResult processSignOut(const SignOutParams& params);

// Construct the JSON body for POST /_matrix/client/r0/login to sign in again.
// Uses the same userId and deviceId.
//
// Original Kotlin (DefaultSignInAgainTask.kt:35-56):
//   signOutAPI.loginAgain(PasswordLoginParams.userIdentifier(
//       user = sessionParams.userId,
//       password = params.password,
//       deviceDisplayName = null,
//       deviceId = sessionParams.deviceId
//   ))
std::string signInAgainBodyToJson(const SignInAgainParams& params);

// Parse login response JSON to extract new credentials.
SignInAgainResult parseLoginResponse(const std::string& json, const std::string& deviceId);

// Check whether a server error should be ignored during sign out.
// The spec allows ignoring M_UNKNOWN_TOKEN (401) errors since the token
// may already be expired — see https://github.com/matrix-org/synapse/issues/5755
bool shouldIgnoreSignOutError(const std::string& errorCode, int httpCode);

} // namespace progressive
