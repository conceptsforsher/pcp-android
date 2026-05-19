#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// Matrix OpenID Token — allows a user to prove ownership of their Matrix
// account to a third-party service.
//
// Ref: https://matrix.org/docs/spec/client_server/latest#post-matrix-client-r0-user-userid-openid-request-token
//
// Original Kotlin (OpenIdToken.kt:24-51):
//   data class OpenIdToken(
//       @Json(name = "access_token")      val accessToken: String,
//       @Json(name = "token_type")        val tokenType: String,
//       @Json(name = "matrix_server_name") val matrixServerName: String,
//       @Json(name = "expires_in")        val expiresIn: Int
//   )
//
// JSON response from POST /_matrix/client/r0/user/{userId}/openid/request_token:
//   {"access_token": "syt_...", "token_type": "Bearer",
//    "matrix_server_name": "example.org", "expires_in": 3600}

struct OpenIdToken {
    std::string accessToken;        // Opaque token for /openid/userinfo
    std::string tokenType;          // Must be "Bearer"
    std::string matrixServerName;   // Homeserver domain for verification
    int expiresIn = 0;              // Seconds until expiration

    bool isValid() const {
        return !accessToken.empty() && tokenType == "Bearer"
            && !matrixServerName.empty() && expiresIn > 0;
    }
};

// Parse OpenID token JSON response
OpenIdToken parseOpenIdToken(const std::string& json);

// Convert OpenID token to JSON
std::string openIdTokenToJson(const OpenIdToken& token);

} // namespace progressive
