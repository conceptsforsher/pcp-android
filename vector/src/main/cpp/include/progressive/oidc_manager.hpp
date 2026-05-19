#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// OIDC/SSO Login Manager — full OIDC authentication flow
//
// Extends the existing oidc_auth module with complete:
//   1. OpenID Connect Discovery (openid-configuration)
//   2. Dynamic Client Registration (POST /register)
//   3. Authorization URL building (with PKCE + state)
//   4. Token exchange (authorization code → access/refresh tokens)
//   5. Token refresh (refresh_token → new access token)
//   6. Token validation (expiry, signature check)
//   7. User info retrieval
//   8. Registration with OIDC token
//   9. SSO provider detection (GitHub, Google, Apple, etc.)
//  10. Well-known fallback detection
//
// Ported from Element Android/X:
//   RustMatrixAuthenticationService.kt
//   OidcAuthenticationService.kt
//   AuthenticationDescriptionProvider.kt
// ================================================================

// ---- Login Type ----

enum class OidcLoginType {
    UNKNOWN = 0,
    PASSWORD = 1,          // m.login.password
    SSO = 2,               // m.login.sso (full page redirect)
    OIDC = 3,              // OIDC/MAS (MSC3861)
    TOKEN = 4,             // m.login.token
    CAS = 5,               // m.login.cas
};

const char* loginTypeToString(OidcLoginType type);
OidcLoginType loginTypeFromString(const std::string& s);

// ---- SSO Provider ----

struct OidcSsoProvider {
    std::string id;              // "github", "google", "apple", etc.
    std::string name;            // "GitHub", "Google", "Apple"
    std::string brand;           // Brand identifier
    std::string iconUrl;         // Provider icon URL
    std::string loginUrl;        // SSO login URL for redirect
};

// Parse SSO providers from login flows response.
std::vector<OidcSsoProvider> oidcParseSsoProviders(const std::string& loginFlowsJson);

// Build SSO login URL for a specific provider.
std::string oidcBuildSsoLoginUrl(const std::string& baseUrl, const std::string& providerId,
                              const std::string& redirectUrl);

// ---- OIDC Configuration ----

struct OidcConfig {
    std::string clientName = "Progressive Chat";
    std::string redirectUri = "chat.progressive.app:/oauth/callback";
    std::string clientUri = "https://progressive.chat";
    std::string logoUri;
    std::string tosUri;
    std::string policyUri;
    std::string scope = "openid urn:matrix:org.matrix.msc2967.client:api:*";
};

// ---- PKCE (RFC 7636) ----

struct OidcPkcePair {
    std::string codeVerifier;        // 43-128 chars random
    std::string codeChallenge;       // SHA-256(verifier), base64url encoded
};

// Generate a cryptographically random PKCE pair.
// Uses std::rand() seeded with time — for production use,
// replace with OpenSSL or OS random generator.
OidcPkcePair oidcGeneratePkce();

// Verify a PKCE code challenge matches the verifier.
bool oidcVerifyPkce(const std::string& verifier, const std::string& challenge);

// ---- OIDC Discovery (OpenID Connect) ----

struct OidcProviderMetadata {
    std::string issuer;
    std::string authorizationEndpoint;
    std::string tokenEndpoint;
    std::string userinfoEndpoint;
    std::string registrationEndpoint;    // Dynamic client registration
    std::string endSessionEndpoint;
    std::string jwksUri;
    std::vector<std::string> scopesSupported;
    std::vector<std::string> responseTypesSupported;
    std::vector<std::string> grantTypesSupported;
    bool supportsDynamicRegistration = false;
    bool valid = false;
    std::string rawJson;                  // Original JSON response
};

// Parse OIDC provider metadata from openid-configuration JSON.
OidcProviderMetadata parseOidcMetadata(const std::string& json);

// ---- Dynamic Client Registration ----

struct OidcClientRegistration {
    std::string clientId;
    std::string clientSecret;
    int64_t clientIdIssuedAt = 0;
    int64_t clientSecretExpiresAt = 0;
    std::vector<std::string> redirectUris;
    std::string tokenEndpointAuthMethod;
    bool valid = false;
};

// Build dynamic client registration request body.
std::string buildClientRegistrationRequest(const OidcConfig& config);

// Parse client registration response.
OidcClientRegistration parseClientRegistration(const std::string& json);

// ---- Authorization ----

struct OidcAuthorization {
    std::string authorizationUrl;    // Full URL to open in browser
    std::string state;               // Opaque state for CSRF
    std::string nonce;               // For ID token verification
    OidcPkcePair pkce;                   // PKCE challenge/verifier pair
    bool valid = false;
};

// Build OIDC authorization URL with PKCE and state.
OidcAuthorization buildOidcAuthorization(const OidcProviderMetadata& metadata,
                                          const OidcClientRegistration& client,
                                          const OidcConfig& config);

// Build state parameter (random opaque string).
std::string generateState();

// Build nonce parameter.
std::string generateNonce();

// ---- Token Exchange ----

struct OidcTokenRequest {
    std::string grantType = "authorization_code";
    std::string code;                // Authorization code from callback
    std::string redirectUri;
    std::string clientId;
    std::string codeVerifier;        // PKCE verifier
};

struct OidcTokenResponseFull {
    std::string accessToken;
    std::string refreshToken;
    std::string idToken;
    std::string tokenType = "Bearer";
    int expiresIn = 0;              // seconds
    std::string scope;
    bool success = false;
    std::string errorMessage;
};

// Build token exchange request body.
std::string buildTokenExchangeRequest(const OidcTokenRequest& req);

// Parse token exchange response.
OidcTokenResponseFull parseTokenResponse(const std::string& json);

// ---- Token Refresh ----

struct OidcRefreshRequest {
    std::string grantType = "refresh_token";
    std::string refreshToken;
    std::string clientId;
    std::string scope;
};

// Build token refresh request body.
std::string buildTokenRefreshRequest(const OidcRefreshRequest& req);

// Check if a token is expired or will expire soon.
// Returns true if token expires in less than `renewThresholdSec` seconds.
bool isTokenExpired(int64_t expiresAtMs, int renewThresholdSec = 300);

// ---- Token Validation ----

struct OidcTokenValidation {
    bool valid = false;
    bool expired = false;
    std::string userId;              // From /whoami
    std::string deviceId;
    std::string homeServer;
    std::string error;
};

// Validate an access token by calling /whoami.
// For client-side: parse the response from the server.
OidcTokenValidation parseWhoamiResponse(const std::string& json);

// ---- Registration with OIDC ----

struct OidcRegistrationRequest {
    std::string username;
    std::string password;            // Optional (can be auto-generated)
    std::string initialDeviceDisplayName;
    bool inhibitLogin = false;       // Don't auto-login after registration
};

// Build registration request body (with OIDC access token in Authorization header).
// This is the /register endpoint, token passed separately.
std::string buildOidcRegistrationRequest(const OidcRegistrationRequest& req);

// ---- SSO Callback Handling ----

// Check if a URL is an SSO/OIDC callback.
bool oidcIsSsoCallbackUrl(const std::string& url);

// Extract the authorization code from a callback URL.
std::string extractAuthCodeFromCallback(const std::string& callbackUrl);

// Extract the state parameter from a callback URL.
std::string extractStateFromCallback(const std::string& callbackUrl);

// ---- Well-Known / Server Discovery ----

struct OidcWellKnownResult {
    std::string baseUrl;
    std::string idServer;
    std::string oidcIssuer;
    bool supportsOidc = false;
    bool supportsPassword = true;
    std::vector<OidcLoginType> supportedLoginTypes;
    std::vector<OidcSsoProvider> ssoProviders;
    std::string error;
};

// Parse well-known response for OIDC and SSO information.
OidcWellKnownResult oidcParseWellKnown(const std::string& json);

// Check if well-known requires OIDC (no password login available).
bool oidcRequiresOidc(const OidcWellKnownResult& wellKnown);

// ---- Login Flows ----

struct OidcLoginFlow {
    OidcLoginType type = OidcLoginType::UNKNOWN;
    std::vector<std::string> identityProviders;  // SSO provider IDs
};

// Parse login flows from GET /login response.
std::vector<OidcLoginFlow> oidcParseLoginFlows(const std::string& json);

// Check if OIDC flow is available.
bool oidcHasOidcFlow(const std::vector<OidcLoginFlow>& flows);

// ---- Password / Token Manager Interface ----

struct LoginCredentials {
    OidcLoginType type = OidcLoginType::PASSWORD;
    std::string userId;              // @user:example.org
    std::string password;
    std::string token;               // For token login
    std::string deviceId;
    std::string initialDeviceDisplayName;
    bool refreshToken = false;       // Request refresh token
};

// Build password login request body.
std::string buildPasswordLoginRequest(const LoginCredentials& creds);

// Build token login request body.
std::string buildTokenLoginRequest(const LoginCredentials& creds);

} // namespace progressive
