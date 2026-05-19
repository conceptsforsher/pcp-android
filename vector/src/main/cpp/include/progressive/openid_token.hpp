#pragma once

#include <string>
#include <cstdint>
#include <ctime>

namespace progressive {

// ============================================================================
// Matrix OpenID Token — allows a user to prove ownership of their Matrix
// account to a third-party service.
// ============================================================================
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

// ============================================================================
// OpenIdCredentials — token with issuance metadata
// ============================================================================
//
// Original Kotlin (OpenIdCredentials.kt):
//   data class OpenIdCredentials(
//       val accessToken: String,
//       val tokenType: String,
//       val expiresIn: Long,
//       val matrixServerName: String,
//       val issuedAtMs: Long
//   )

struct OpenIdCredentials {
    std::string accessToken;
    std::string tokenType;         // "Bearer"
    int64_t expiresIn = 0;        // Seconds until expiration
    std::string matrixServerName;
    int64_t issuedAtMs = 0;       // Unix timestamp (ms) when token was issued

    // Check if the token has expired based on issuance time.
    bool isExpired() const;
    bool isValid() const;
};

// ============================================================================
// OpenIdTokenResponse — raw server response wrapper
// ============================================================================
//
// Original Kotlin (RequestOpenIdTokenResponse.kt:23-48):
//   @JsonClass(generateAdapter = true)
//   internal data class RequestOpenIdTokenResponse(
//       @Json(name = "access_token")       val openIdToken: String,
//       @Json(name = "token_type")         val tokenType: String,
//       @Json(name = "matrix_server_name") val matrixServerName: String,
//       @Json(name = "expires_in")         val expiresIn: Int
//   )

struct OpenIdTokenResponse {
    std::string accessToken;
    std::string tokenType;
    std::string matrixServerName;
    int64_t expiresIn = 0;

    OpenIdToken toToken() const;
    OpenIdCredentials toCredentials() const;
    bool isValid() const;
};

// ============================================================================
// OpenID Token Request / Response (manual JSON)
// ============================================================================

// Build the JSON request body for POST /user/{userId}/openid/request_token.
// The body is an empty JSON object {}.
//
// Original Kotlin (OpenIdAPI.kt:37-41):
//   @POST("user/{userId}/openid/request_token")
//   suspend fun openIdToken(
//       @Path("userId") userId: String,
//       @Body body: JsonDict = emptyMap()
//   ): OpenIdToken
std::string buildOpenIdTokenRequest(const std::string& userId);

// Build the full endpoint URL.
std::string buildOpenIdTokenEndpoint(const std::string& homeserverBase,
                                      const std::string& userId);

// Parse the OpenID token JSON response into OpenIdTokenResponse.
//
// Original Kotlin (GetOpenIdTokenTask.kt:34-37):
//   override suspend fun execute(params: Unit): OpenIdToken {
//       return executeRequest(globalErrorReceiver) {
//           openIdAPI.openIdToken(userId)
//       }
//   }
OpenIdTokenResponse parseOpenIdTokenResponse(const std::string& responseJson);

// Convert OpenIdTokenResponse to JSON.
std::string openIdTokenResponseToJson(const OpenIdTokenResponse& response);

// Check if a token is expired (based on current time and expiresIn).
// Returns true if the token has expired or is invalid.
//
// Original Kotlin (OpenIdToken.kt utility):
//   fun isTokenExpired(token: OpenIdToken, issuedAtMs: Long): Boolean
bool isTokenExpired(const OpenIdCredentials& credentials);
bool isTokenExpired(const OpenIdToken& token, int64_t issuedAtMs);

// ============================================================================
// Third-Party Lookup (Identity Server 3PID lookup)
// ============================================================================
//
// Original Kotlin (ThirdPartyLookup.kt):
//   Used to look up a Matrix user by email/phone via identity server.

// Build the JSON request for 3PID lookup on identity server.
// POST /_matrix/identity/v2/lookup
//
// Original Kotlin (IdentityAPI.kt):
//   @POST("/_matrix/identity/v2/lookup")
//   suspend fun lookup(@Body body: LookupBody): LookupResponse

struct ThirdPartyLookupRequest {
    std::string address;           // Email address or phone number
    std::string medium;            // "email" or "msisdn"
    std::string idAccessToken;     // OpenID access token for authentication
    std::string idServer;          // Identity server base URL

    bool isValid() const {
        return !address.empty() && !medium.empty() && !idAccessToken.empty();
    }
};

// Build JSON for third-party lookup request.
std::string buildThirdPartyLookupRequest(const ThirdPartyLookupRequest& request);

// Build the lookup endpoint URL.
std::string buildThirdPartyLookupEndpoint(const std::string& idServer);

// Third-party lookup response.
struct ThirdPartyLookupResponse {
    bool found = false;            // Whether a matching user was found
    std::string mxid;              // The Matrix user ID found (e.g. "@alice:example.org")
    std::string displayName;       // Display name from identity server
    std::string avatarUrl;         // Avatar URL from identity server
    bool success = false;          // Whether the request succeeded

    bool isMatched() const { return found && !mxid.empty(); }
};

// Parse third-party lookup response JSON.
//
// Original Kotlin (LookupResponse.kt):
//   Expects: {"address": "...", "medium": "...", "mxid": "...", "display_name": "..."}
ThirdPartyLookupResponse parseThirdPartyLookupResponse(const std::string& responseJson);

// Convert ThirdPartyLookupResponse to JSON.
std::string thirdPartyLookupResponseToJson(const ThirdPartyLookupResponse& response);

// ============================================================================
// Widget OpenID — Widget OpenID Connect authentication
// ============================================================================
//
// Original Kotlin (WidgetOpenId.kt / WidgetAPI.kt):
//   data class WidgetOpenIdRequest(val widgetId: String)
//   data class WidgetOpenIdResponse(
//       val accessToken: String,
//       val tokenType: String,
//       val matrixServerName: String,
//       val expiresIn: Long
//   )
//
// Widgets use OpenID to authenticate the user when integrating
// with third-party services (e.g. Jitsi, Etherpad, etc.).

struct WidgetOpenIdRequest {
    std::string widgetId;          // The widget requesting authentication
    std::string roomId;            // Room ID for context
    std::string userId;            // User ID for authentication
    std::string clientSecret;      // Optional client secret
    std::string redirectUri;       // Optional redirect URI
    std::string state;             // Optional state parameter for CSRF

    bool isValid() const {
        return !widgetId.empty() && !roomId.empty() && !userId.empty();
    }
};

// Build JSON for widget OpenID request (sent to the widget).
std::string buildWidgetOpenIdRequest(const WidgetOpenIdRequest& request);

// Widget OpenID response from the Matrix server.
struct WidgetOpenIdResponse {
    std::string accessToken;
    std::string tokenType;         // "Bearer"
    std::string matrixServerName;
    int64_t expiresIn = 0;
    std::string widgetId;          // Echoed back for correlation

    bool isValid() const {
        return !accessToken.empty() && tokenType == "Bearer"
            && !matrixServerName.empty() && expiresIn > 0;
    }

    OpenIdToken toOpenIdToken() const;
    OpenIdCredentials toCredentials() const;
};

// Build widget OpenID response JSON.
std::string widgetOpenIdResponseToJson(const WidgetOpenIdResponse& response);

// Parse widget OpenID response from JSON.
WidgetOpenIdResponse parseWidgetOpenIdResponse(const std::string& responseJson);

// ============================================================================
// Utility: construct widget's OpenID callback URL
// ============================================================================

// Build the OpenID callback URL for a widget.
// Widgets call back to this URL with the authorization code.
std::string buildWidgetOpenIdCallback(const std::string& homeserverBase,
                                       const std::string& widgetId,
                                       const std::string& userId);

// Build the OpenID userinfo verification URL.
// Third-party services call this to verify the token.
//
// Ref: https://matrix.org/docs/spec/client_server/latest#get-matrix-client-r0-user-userid-openid-userinfo
std::string buildOpenIdUserinfoUrl(const std::string& homeserverBase,
                                    const std::string& userId,
                                    const std::string& accessToken);

} // namespace progressive
