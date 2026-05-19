#ifndef PROGRESSIVE_SSO_UTILS_HPP
#define PROGRESSIVE_SSO_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- SSO / Login Token Utilities ----

struct SsoRedirect {
    std::string url;
    std::string provider;       // "google", "github", "apple", etc.
    std::string brand;          // "Google", "GitHub", "Apple"
    bool valid = false;
};

struct LoginToken {
    std::string token;
    std::string loginToken;    // m.login.token value
    int64_t expiresInMs = 0;
    int64_t expiresAtMs = 0;
    bool valid = false;
};

// Parse SSO redirect URL from Matrix login flows response.
std::vector<SsoRedirect> parseSsoRedirects(const std::string& loginFlowsJson);

// Parse login token from SSO callback URL.
LoginToken parseLoginToken(const std::string& callbackUrl);

// Check if a URL is a valid SSO callback.
bool isSsoCallbackUrl(const std::string& url);

// Build the SSO login URL with redirect parameters.
std::string buildSsoLoginUrl(const std::string& baseUrl, const std::string& redirectUrl);

// Extract the provider name from an SSO identity provider ID.
std::string extractSsoProvider(const std::string& idpId);

// Get a human-readable brand name for an SSO provider.
std::string getSsoProviderBrand(const std::string& provider);

// Check if a login token has expired.
bool isLoginTokenExpired(const LoginToken& token);

// Format SSO providers list as JSON.
std::string ssoProvidersToJson(const std::vector<SsoRedirect>& providers);

// ---- Homeserver URL Utilities ----

struct HomeserverUrl {
    std::string rawUrl;
    std::string sanitizedUrl;
    std::string serverName;     // "matrix.org"
    int port = 443;
    bool isHttps = true;
    bool valid = false;
    std::string errorMessage;
};

// Validate and sanitize a homeserver URL entered by the user.
HomeserverUrl validateHomeserverUrl(const std::string& input);

// Auto-detect if a URL is a homeserver or identity server.
bool isHomeserverUrl(const std::string& url);

} // namespace progressive

#endif // PROGRESSIVE_SSO_UTILS_HPP
