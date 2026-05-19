#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ==== OIDC/MAS Authentication (Matrix Authentication Service) ====
//
// Implements the OIDC-based registration/login flow per MSC3861.
// Replaces Element X's Rust SDK calls with native C++ via our HTTP client.
//
// Flow:
//   1. Server discovery: check .well-known for OIDC issuer
//   2. Dynamic client registration (if needed)
//   3. Build OAuth authorization URL with PKCE challenge
//   4. User completes OAuth in browser → callback URL
//   5. Exchange authorization code for access token
//
// Original source: element-x-android RustMatrixAuthenticationService.kt

// OAuth configuration for dynamic client registration.
struct OAuthConfig {
    std::string clientName;          // "Progressive Chat"
    std::string redirectUri;         // "chat.progressive.app:/" (custom scheme)
    std::string clientUri;           // "https://progressive.chat"
    std::string logoUri;             // app logo URL
    std::string tosUri;              // terms of service
    std::string policyUri;           // privacy policy
};

// Result of homeserver discovery for OIDC support.
struct OidcDiscovery {
    bool supportsOidc = false;       // Homeserver supports OAuth login
    bool supportsPassword = true;    // Password login still available
    std::string issuer;              // OIDC issuer URL (e.g. "https://auth.matrix.org")
    std::string authorizationEndpoint;
    std::string tokenEndpoint;
    std::string registrationEndpoint;  // Dynamic client registration
    std::string accountManagementUri;  // MSC4191
    std::string errorMessage;
};

// PKCE (Proof Key for Code Exchange) pair.
struct PkcePair {
    std::string codeVerifier;        // 43-128 chars, [A-Za-z0-9\-._~]
    std::string codeChallenge;       // SHA-256(codeVerifier), base64url
};

// OAuth authorization details (what to show the user).
struct OAuthDetails {
    std::string authorizationUrl;    // Full OIDC authorization URL to open in browser
    std::string state;               // Opaque state parameter (for CSRF protection)
    bool isValid = false;
};

// Token response from token endpoint.
struct OidcTokenResponse {
    std::string accessToken;
    std::string refreshToken;
    std::string userId;              // From Matrix /whoami after getting token
    std::string deviceId;
    std::string homeServer;
    int expiresIn = 0;              // seconds
    bool success = false;
    std::string errorMessage;
};

// ==== Server Discovery ====

// Discover OIDC support by fetching .well-known/matrix/client.
// Returns OidcDiscovery with issuer URL if OIDC is supported.
//
// Original Kotlin: setHomeserver() → client.homeserverLoginDetails()
OidcDiscovery discoverOidc(const std::string& homeserverUrl);

// Check if a homeserver supports OIDC login based on its well-known.
bool supportsOidcLogin(const std::string& homeserverUrl);

// ==== OAuth Metadata ====

// GET /auth_metadata (MSC2965) — fetch OIDC issuer metadata.
// Returns authorization_endpoint, token_endpoint, registration_endpoint.
OidcDiscovery fetchOAuthMetadata(const std::string& issuerUrl);

// ==== PKCE ====

// Generate a cryptographically random PKCE code verifier and challenge.
// Uses SHA-256 for the challenge per RFC 7636.
PkcePair generatePkce();

// ==== OAuth URL Building ====

// Build the full OIDC authorization URL.
// Parameters: issuer, clientId (from registration), redirectUri, state, pkceChallenge
//
// Original Kotlin: getOAuthUrl(prompt) → client.urlForOauth()
//   prompt = "login" for sign-in, "create" for registration
std::string buildOAuthAuthorizationUrl(
    const OidcDiscovery& discovery,
    const std::string& clientId,
    const std::string& redirectUri,
    const std::string& state,
    const std::string& pkceChallenge,
    const std::string& prompt = "login",   // "login" or "create"
    const std::string& loginHint = ""
);

// ==== Dynamic Client Registration ====

// POST to registration_endpoint to dynamically register this client.
// Returns client_id. If static registration is configured, returns that instead.
//
// Original Kotlin: OAuthConfigurationProvider → OAuthConfiguration
std::string registerOidcClient(
    const std::string& registrationEndpoint,
    const OAuthConfig& config
);

// ==== Token Exchange ====

// Exchange the authorization code + PKCE verifier for tokens.
// POST to token_endpoint.
//
// Original Kotlin: loginWithOAuth(callbackUrl) → client.loginWithOauthCallback()
OidcTokenResponse exchangeOidcCode(
    const std::string& tokenEndpoint,
    const std::string& clientId,
    const std::string& redirectUri,
    const std::string& code,
    const std::string& codeVerifier
);

// ==== Callback URL Parser ====

// Parse the OIDC callback URL to determine success/failure.
// Format: chat.progressive.app:/?state=...&code=... (success)
//         chat.progressive.app:/?error=access_denied&state=... (canceled)
//
// Original Kotlin: DefaultOAuthUrlParser.parse(url)

enum class OAuthAction { NONE = 0, SUCCESS = 1, GO_BACK = 2 };

struct ParsedOAuthCallback {
    OAuthAction action = OAuthAction::NONE;
    std::string fullUrl;            // The complete callback URL (for token exchange)
    std::string code;               // Authorization code (if SUCCESS)
    std::string state;              // State parameter for verification
};

ParsedOAuthCallback parseOAuthCallback(const std::string& url, const std::string& redirectUri);

// ==== Utility ====

// Generate a random state string for CSRF protection.
std::string generateOAuthState();

} // namespace progressive
