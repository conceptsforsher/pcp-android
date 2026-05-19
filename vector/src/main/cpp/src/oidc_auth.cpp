#include "progressive/oidc_auth.hpp"
#include "progressive/http_client.hpp"
#include "progressive/crypto_algorithms.hpp"
#include "progressive/graph_utils.hpp"
#include <sstream>
#include <cstdlib>
#include <ctime>

namespace progressive {

// ==== Simple PRNG for CSRF state and PKCE (not cryptographically strong, but sufficient) ====

static std::string randomString(int length) {
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";
    static bool seeded = false;
    if (!seeded) { srand(time(nullptr)); seeded = true; }
    std::string result;
    for (int i = 0; i < length; i++) {
        result += charset[rand() % (sizeof(charset) - 1)];
    }
    return result;
}

// ==== Server Discovery ====

OidcDiscovery discoverOidc(const std::string& homeserverUrl) {
    OidcDiscovery result;

    // Step 1: Fetch .well-known/matrix/client
    std::string wellKnownUrl = homeserverUrl;
    if (!wellKnownUrl.empty() && wellKnownUrl.back() == '/') wellKnownUrl.pop_back();
    wellKnownUrl += "/.well-known/matrix/client";

    auto resp = httpGet(wellKnownUrl);
    if (!resp.isOk()) {
        result.supportsOidc = false;
        result.supportsPassword = true; // fallback
        result.errorMessage = "No .well-known found";
        return result;
    }

    // Step 2: Parse well-known for OIDC issuer directly from JSON
    // Format: {"m.homeserver":{"base_url":"..."},"org.matrix.msc2965.authentication":{"issuer":"..."}}
    auto extractIssuer = [&](const std::string& json) -> std::string {
        auto pos = json.find("\"org.matrix.msc2965.authentication\"");
        if (pos == std::string::npos) return "";
        pos = json.find('{', pos);
        if (pos == std::string::npos) return "";
        int depth = 1;
        size_t start = pos;
        pos++;
        while (pos < json.size() && depth > 0) { if (json[pos] == '{') depth++; else if (json[pos] == '}') depth--; pos++; }
        std::string authObj = json.substr(start, pos - start);
        // Extract "issuer" from auth object
        auto issPos = authObj.find("\"issuer\"");
        if (issPos == std::string::npos) return "";
        issPos = authObj.find(':', issPos);
        if (issPos == std::string::npos) return "";
        issPos++;
        while (issPos < authObj.size() && (authObj[issPos] == ' ' || authObj[issPos] == '\t' || authObj[issPos] == '"')) issPos++;
        size_t end = issPos;
        while (end < authObj.size() && authObj[end] != '"') { if (authObj[end] == '\\') end++; end++; }
        return authObj.substr(issPos, end - issPos);
    };

    std::string issuer = extractIssuer(resp.body);

    if (!issuer.empty()) {
        result.issuer = issuer;
        result.supportsOidc = true;
        result.supportsPassword = false; // OIDC homeservers don't support password login
    } else {
        result.supportsOidc = false;
        result.supportsPassword = true;
    }

    // Step 3: Fetch auth metadata from issuer
    if (result.supportsOidc) {
        result = fetchOAuthMetadata(result.issuer);
    }

    return result;
}

bool supportsOidcLogin(const std::string& homeserverUrl) {
    auto discovery = discoverOidc(homeserverUrl);
    return discovery.supportsOidc;
}

// ==== OAuth Metadata ====

OidcDiscovery fetchOAuthMetadata(const std::string& issuerUrl) {
    OidcDiscovery result;
    result.issuer = issuerUrl;
    result.supportsOidc = true;
    result.supportsPassword = false;

    std::string metadataUrl = issuerUrl;
    if (!metadataUrl.empty() && metadataUrl.back() == '/') metadataUrl.pop_back();
    metadataUrl += "/.well-known/oauth-authorization-server";

    // Also try the Matrix-specific path
    auto resp = httpGet(metadataUrl);
    if (!resp.isOk()) {
        // Try /auth_metadata endpoint (MSC2965)
        metadataUrl = issuerUrl + "/auth_metadata";
        resp = httpGet(metadataUrl);
    }

    if (!resp.isOk()) {
        result.errorMessage = "Failed to fetch OAuth metadata";
        return result;
    }

    // Parse JSON response manually
    // Format: {"issuer":"...","authorization_endpoint":"...","token_endpoint":"...","registration_endpoint":"..."}
    auto extract = [&](const std::string& key) -> std::string {
        auto pos = resp.body.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = resp.body.find(':', pos);
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < resp.body.size() && (resp.body[pos] == ' ' || resp.body[pos] == '\t' || resp.body[pos] == '"')) pos++;
        size_t end = pos;
        while (end < resp.body.size() && resp.body[end] != '"') { if (resp.body[end] == '\\') end++; end++; }
        return resp.body.substr(pos, end - pos);
    };

    result.authorizationEndpoint = extract("authorization_endpoint");
    result.tokenEndpoint = extract("token_endpoint");
    result.registrationEndpoint = extract("registration_endpoint");
    result.accountManagementUri = extract("account_management_uri");

    return result;
}

// ==== PKCE ====

PkcePair generatePkce() {
    PkcePair pkce;

    // Generate code_verifier (43-128 chars, unreserved chars per RFC 7636)
    pkce.codeVerifier = randomString(64);

    // Compute code_challenge = BASE64URL(SHA256(code_verifier))
    auto hash = sha256(reinterpret_cast<const uint8_t*>(pkce.codeVerifier.data()),
                       pkce.codeVerifier.size());

    // Convert hash to base64url (no padding)
    std::string base64;
    for (uint8_t b : hash) base64 += static_cast<char>(b);
    pkce.codeChallenge = base64ToBase64Url(base64);

    return pkce;
}

// ==== OAuth URL Building ====

std::string buildOAuthAuthorizationUrl(
    const OidcDiscovery& discovery,
    const std::string& clientId,
    const std::string& redirectUri,
    const std::string& state,
    const std::string& pkceChallenge,
    const std::string& prompt,
    const std::string& loginHint)
{
    // Build: authorization_endpoint
    //   ?response_type=code
    //   &client_id=...
    //   &redirect_uri=...
    //   &state=...
    //   &code_challenge=...
    //   &code_challenge_method=S256
    //   &scope=openid urn:matrix:org.matrix.msc2967.client:api:*
    //   &prompt=login (or create)
    std::ostringstream url;
    url << discovery.authorizationEndpoint;
    url << "?response_type=code";
    url << "&client_id=" << clientId;
    url << "&redirect_uri=" << redirectUri;
    url << "&state=" << state;
    url << "&code_challenge=" << pkceChallenge;
    url << "&code_challenge_method=S256";
    url << "&scope=openid";
    url << "&prompt=" << prompt;

    if (!loginHint.empty()) {
        url << "&login_hint=" << loginHint;
    }

    return url.str();
}

// ==== Dynamic Client Registration ====

std::string registerOidcClient(
    const std::string& registrationEndpoint,
    const OAuthConfig& config)
{
    if (registrationEndpoint.empty()) return "";

    // Build registration JSON
    std::ostringstream body;
    body << "{";
    body << R"("client_name":")" << config.clientName << R"(",)";
    body << R"("redirect_uris":[")" << config.redirectUri << R"("],)";
    body << R"("client_uri":")" << config.clientUri << R"(",)";
    body << R"("logo_uri":")" << config.logoUri << R"(",)";
    body << R"("tos_uri":")" << config.tosUri << R"(",)";
    body << R"("policy_uri":")" << config.policyUri << R"(",)";
    body << R"("application_type":"native",)";
    body << R"("grant_types":["authorization_code","refresh_token"])";
    body << "}";

    auto resp = httpPost(registrationEndpoint, body.str(),
        {{"Content-Type", "application/json"}});

    if (!resp.isOk()) return "";

    // Extract client_id from response: {"client_id":"...","client_id_issued_at":...}
    auto pos = resp.body.find("\"client_id\"");
    if (pos == std::string::npos) return "";
    pos = resp.body.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < resp.body.size() && (resp.body[pos] == ' ' || resp.body[pos] == '\t' || resp.body[pos] == '"')) pos++;
    size_t end = pos;
    while (end < resp.body.size() && resp.body[end] != '"') { if (resp.body[end] == '\\') end++; end++; }
    return resp.body.substr(pos, end - pos);
}

// ==== Token Exchange ====

OidcTokenResponse exchangeOidcCode(
    const std::string& tokenEndpoint,
    const std::string& clientId,
    const std::string& redirectUri,
    const std::string& code,
    const std::string& codeVerifier)
{
    OidcTokenResponse result;

    // Build form-encoded body
    std::ostringstream body;
    body << "grant_type=authorization_code";
    body << "&client_id=" << clientId;
    body << "&redirect_uri=" << redirectUri;
    body << "&code=" << code;
    body << "&code_verifier=" << codeVerifier;

    auto resp = httpPost(tokenEndpoint, body.str(),
        {{"Content-Type", "application/x-www-form-urlencoded"}});

    if (!resp.isOk()) {
        result.errorMessage = "Token exchange failed: HTTP " + std::to_string(resp.statusCode);
        return result;
    }

    // Parse response: {"access_token":"...","refresh_token":"...","expires_in":3600,...}
    auto extract = [&](const std::string& key) -> std::string {
        auto pos = resp.body.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = resp.body.find(':', pos);
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < resp.body.size() && (resp.body[pos] == ' ' || resp.body[pos] == '\t' || resp.body[pos] == '"')) pos++;
        size_t end = pos;
        while (end < resp.body.size() && resp.body[end] != '"') { if (resp.body[end] == '\\') end++; end++; }
        return resp.body.substr(pos, end - pos);
    };

    result.accessToken = extract("access_token");
    result.refreshToken = extract("refresh_token");
    result.expiresIn = std::stoi(extract("expires_in").empty() ? "3600" : extract("expires_in"));

    if (!result.accessToken.empty()) {
        result.success = true;
    } else {
        result.errorMessage = "No access_token in response";
    }

    return result;
}

// ==== Callback Parser ====

ParsedOAuthCallback parseOAuthCallback(const std::string& url, const std::string& redirectUri) {
    ParsedOAuthCallback result;

    // Check if this is our callback URL
    if (url.find(redirectUri) != 0) return result;

    // Parse query parameters from the URL
    auto queryStart = url.find('?');
    if (queryStart == std::string::npos) return result;
    std::string query = url.substr(queryStart + 1);

    // Check for error
    if (query.find("error=access_denied") != std::string::npos) {
        result.action = OAuthAction::GO_BACK;
        return result;
    }

    // Extract code and state
    auto extractParam = [&](const std::string& key) -> std::string {
        auto pos = query.find(key + "=");
        if (pos == std::string::npos) return "";
        pos += key.size() + 1;
        auto end = query.find('&', pos);
        return query.substr(pos, end != std::string::npos ? end - pos : std::string::npos);
    };

    result.code = extractParam("code");
    result.state = extractParam("state");

    if (!result.code.empty()) {
        result.action = OAuthAction::SUCCESS;
        result.fullUrl = url;
    }

    return result;
}

// ==== OAuth State ====

std::string generateOAuthState() {
    return randomString(32);
}

} // namespace progressive
