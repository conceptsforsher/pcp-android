#include "progressive/oidc_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

namespace progressive {

// ====== JSON helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static int64_t extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Login Type ======

const char* loginTypeToString(OidcLoginType type) {
    switch (type) {
        case OidcLoginType::PASSWORD: return "m.login.password";
        case OidcLoginType::SSO: return "m.login.sso";
        case OidcLoginType::OIDC: return "m.login.oidc";
        case OidcLoginType::TOKEN: return "m.login.token";
        case OidcLoginType::CAS: return "m.login.cas";
        default: return "unknown";
    }
}

OidcLoginType loginTypeFromString(const std::string& s) {
    if (s == "m.login.password") return OidcLoginType::PASSWORD;
    if (s == "m.login.sso") return OidcLoginType::SSO;
    if (s == "m.login.oidc") return OidcLoginType::OIDC;
    if (s == "m.login.token") return OidcLoginType::TOKEN;
    if (s == "m.login.cas") return OidcLoginType::CAS;
    return OidcLoginType::UNKNOWN;
}

// ====== SSO Providers ======

std::vector<OidcSsoProvider> oidcParseSsoProviders(const std::string& loginFlowsJson) {
    std::vector<OidcSsoProvider> providers;
    size_t pos = 0;

    // Look for identity_providers array within m.login.sso flows
    while ((pos = loginFlowsJson.find("\"identity_providers\"", pos)) != std::string::npos) {
        pos = loginFlowsJson.find('[', pos);
        if (pos == std::string::npos) break;
        pos++;

        while (pos < loginFlowsJson.size()) {
            while (pos < loginFlowsJson.size() && (loginFlowsJson[pos] == ' ' || loginFlowsJson[pos] == ',' || loginFlowsJson[pos] == '\n')) pos++;
            if (pos >= loginFlowsJson.size() || loginFlowsJson[pos] == ']') break;

            size_t objEnd = loginFlowsJson.find('}', pos);
            if (objEnd == std::string::npos) break;
            std::string obj = loginFlowsJson.substr(pos, objEnd - pos + 1);

            OidcSsoProvider p;
            p.id = extractStr(obj, "id");
            p.name = extractStr(obj, "name");
            p.brand = extractStr(obj, "brand");
            p.iconUrl = extractStr(obj, "icon");
            if (!p.id.empty()) providers.push_back(p);

            pos = objEnd + 1;
        }
        break; // Only first identity_providers
    }

    return providers;
}

std::string oidcBuildSsoLoginUrl(const std::string& baseUrl, const std::string& providerId,
                              const std::string& redirectUrl) {
    std::ostringstream url;
    url << baseUrl;
    if (baseUrl.back() != '/') url << "/";
    url << "_matrix/client/r0/login/sso/redirect/" << providerId
        << "?redirectUrl=" << redirectUrl;
    return url.str();
}

// ====== PKCE ======

static std::string base64UrlEncode(const std::string& input) {
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    for (size_t i = 0; i < input.size(); i += 3) {
        uint32_t t = static_cast<uint8_t>(input[i]) << 16;
        if (i+1 < input.size()) t |= static_cast<uint8_t>(input[i+1]) << 8;
        if (i+2 < input.size()) t |= static_cast<uint8_t>(input[i+2]);
        encoded += b64[(t >> 18) & 0x3F];
        encoded += b64[(t >> 12) & 0x3F];
        encoded += (i+1 < input.size()) ? b64[(t >> 6) & 0x3F] : '=';
        encoded += (i+2 < input.size()) ? b64[t & 0x3F] : '=';
    }
    // Base64 → Base64url: + → -, / → _, trim =
    std::string result;
    for (char c : encoded) {
        if (c == '+') result += '-';
        else if (c == '/') result += '_';
        else if (c != '=') result += c;
    }
    return result;
}

OidcPkcePair oidcGeneratePkce() {
    OidcPkcePair pair;
    // Generate 64 random chars for code verifier
    static const char* valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";
    srand(static_cast<unsigned>(time(nullptr)) * 100 + rand());
    for (int i = 0; i < 64; i++) {
        pair.codeVerifier += valid[rand() % 66];
    }

    // SHA-256 hash (mock — real impl needs OpenSSL)
    // For production: use SHA-256(codeVerifier) then base64url
    // Mock: use base64url of the verifier itself
    std::string mockHash = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    pair.codeChallenge = mockHash.substr(0, 43);

    return pair;
}

bool oidcVerifyPkce(const std::string& verifier, const std::string& challenge) {
    // Real implementation: SHA-256(verifier) → base64url → compare with challenge
    (void)verifier; (void)challenge;
    return true; // Mock
}

// ====== OIDC Discovery ======

OidcProviderMetadata parseOidcMetadata(const std::string& json) {
    OidcProviderMetadata meta;
    meta.issuer = extractStr(json, "issuer");
    meta.authorizationEndpoint = extractStr(json, "authorization_endpoint");
    meta.tokenEndpoint = extractStr(json, "token_endpoint");
    meta.userinfoEndpoint = extractStr(json, "userinfo_endpoint");
    meta.registrationEndpoint = extractStr(json, "registration_endpoint");
    meta.endSessionEndpoint = extractStr(json, "end_session_endpoint");
    meta.jwksUri = extractStr(json, "jwks_uri");

    // Parse scopes_supported array
    size_t pos = json.find("\"scopes_supported\"");
    if (pos != std::string::npos) {
        pos = json.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') { pos++; size_t e = pos;
                    while (e < json.size() && json[e] != '"') e++;
                    meta.scopesSupported.push_back(json.substr(pos, e - pos));
                    pos = e; }
                pos++;
            }
        }
    }

    meta.supportsDynamicRegistration = !meta.registrationEndpoint.empty();
    meta.valid = !meta.issuer.empty() && !meta.authorizationEndpoint.empty() && !meta.tokenEndpoint.empty();
    meta.rawJson = json;

    return meta;
}

// ====== Dynamic Client Registration ======

std::string buildClientRegistrationRequest(const OidcConfig& config) {
    std::ostringstream os;
    os << R"({"client_name":")" << config.clientName
       << R"(","redirect_uris":[")" << config.redirectUri << R"("])";
    if (!config.clientUri.empty()) os << R"(,"client_uri":")" << config.clientUri << R"(")";
    if (!config.logoUri.empty()) os << R"(,"logo_uri":")" << config.logoUri << R"(")";
    if (!config.tosUri.empty()) os << R"(,"tos_uri":")" << config.tosUri << R"(")";
    if (!config.policyUri.empty()) os << R"(,"policy_uri":")" << config.policyUri << R"(")";
    os << R"(,"application_type":"native")";
    os << R"(,"grant_types":["authorization_code","refresh_token"])";
    os << R"(,"response_types":["code"])";
    os << R"(,"token_endpoint_auth_method":"none")"; // PKCE public client
    os << "}";
    return os.str();
}

OidcClientRegistration parseClientRegistration(const std::string& json) {
    OidcClientRegistration reg;
    reg.clientId = extractStr(json, "client_id");
    reg.clientSecret = extractStr(json, "client_secret");
    reg.clientIdIssuedAt = extractInt(json, "client_id_issued_at");
    reg.clientSecretExpiresAt = extractInt(json, "client_secret_expires_at");
    reg.tokenEndpointAuthMethod = extractStr(json, "token_endpoint_auth_method");

    // Parse redirect_uris
    size_t pos = json.find("\"redirect_uris\"");
    if (pos != std::string::npos) {
        pos = json.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') { pos++; size_t e = pos;
                    while (e < json.size() && json[e] != '"') e++;
                    reg.redirectUris.push_back(json.substr(pos, e - pos));
                    pos = e; }
                pos++;
            }
        }
    }

    reg.valid = !reg.clientId.empty();
    return reg;
}

// ====== Authorization ======

std::string generateState() {
    srand(static_cast<unsigned>(time(nullptr)) + rand());
    static const char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string state;
    for (int i = 0; i < 32; i++) state += chars[rand() % 62];
    return state;
}

std::string generateNonce() {
    return generateState(); // Same pattern
}

OidcAuthorization buildOidcAuthorization(const OidcProviderMetadata& metadata,
                                          const OidcClientRegistration& client,
                                          const OidcConfig& config) {
    OidcAuthorization auth;
    if (!metadata.valid || !client.valid) return auth;

    auth.pkce = oidcGeneratePkce();
    auth.state = generateState();
    auth.nonce = generateNonce();

    std::ostringstream url;
    url << metadata.authorizationEndpoint
        << "?response_type=code"
        << "&client_id=" << client.clientId
        << "&redirect_uri=" << config.redirectUri
        << "&scope=" << config.scope
        << "&state=" << auth.state
        << "&nonce=" << auth.nonce
        << "&code_challenge=" << auth.pkce.codeChallenge
        << "&code_challenge_method=S256";

    auth.authorizationUrl = url.str();
    auth.valid = true;
    return auth;
}

// ====== Token Exchange ======

std::string buildTokenExchangeRequest(const OidcTokenRequest& req) {
    std::ostringstream os;
    os << "grant_type=" << req.grantType
       << "&code=" << req.code
       << "&redirect_uri=" << req.redirectUri
       << "&client_id=" << req.clientId
       << "&code_verifier=" << req.codeVerifier;
    return os.str();
}

OidcTokenResponseFull parseTokenResponse(const std::string& json) {
    OidcTokenResponseFull resp;
    resp.accessToken = extractStr(json, "access_token");
    resp.refreshToken = extractStr(json, "refresh_token");
    resp.idToken = extractStr(json, "id_token");
    resp.tokenType = extractStr(json, "token_type");
    if (resp.tokenType.empty()) resp.tokenType = "Bearer";
    resp.expiresIn = static_cast<int>(extractInt(json, "expires_in"));
    resp.scope = extractStr(json, "scope");

    auto err = extractStr(json, "error");
    if (!err.empty()) {
        resp.errorMessage = err + ": " + extractStr(json, "error_description");
        resp.success = false;
    } else {
        resp.success = !resp.accessToken.empty();
    }

    return resp;
}

// ====== Token Refresh ======

std::string buildTokenRefreshRequest(const OidcRefreshRequest& req) {
    std::ostringstream os;
    os << "grant_type=" << req.grantType
       << "&refresh_token=" << req.refreshToken
       << "&client_id=" << req.clientId;
    if (!req.scope.empty()) os << "&scope=" << req.scope;
    return os.str();
}

bool isTokenExpired(int64_t expiresAtMs, int renewThresholdSec) {
    auto now = static_cast<int64_t>(time(nullptr)) * 1000;
    return now + (renewThresholdSec * 1000LL) >= expiresAtMs;
}

// ====== Token Validation ======

OidcTokenValidation parseWhoamiResponse(const std::string& json) {
    OidcTokenValidation val;
    val.userId = extractStr(json, "user_id");
    val.deviceId = extractStr(json, "device_id");
    if (val.deviceId.empty()) val.deviceId = extractStr(json, "deviceId");
    val.homeServer = extractStr(json, "home_server");
    val.valid = !val.userId.empty();

    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        val.valid = false;
        val.error = err + ": " + extractStr(json, "error");
    }

    return val;
}

// ====== Registration with OIDC ======

std::string buildOidcRegistrationRequest(const OidcRegistrationRequest& req) {
    std::ostringstream os;
    os << R"({"username":")" << req.username << R"(")";
    os << R"(,"initial_device_display_name":")" << req.initialDeviceDisplayName << R"(")";
    if (!req.password.empty()) os << R"(,"password":")" << req.password << R"(")";
    if (req.inhibitLogin) os << R"(,"inhibit_login":true)";
    os << "}";
    return os.str();
}

// ====== SSO Callback ======

bool oidcIsSsoCallbackUrl(const std::string& url) {
    return url.find("login/sso/redirect") != std::string::npos ||
           url.find("code=") != std::string::npos ||
           url.find("oauth/callback") != std::string::npos;
}

std::string extractAuthCodeFromCallback(const std::string& callbackUrl) {
    auto pos = callbackUrl.find("code=");
    if (pos == std::string::npos) return "";

    pos += 5; // Skip "code="
    auto end = callbackUrl.find('&', pos);
    if (end == std::string::npos) end = callbackUrl.size();
    return callbackUrl.substr(pos, end - pos);
}

std::string extractStateFromCallback(const std::string& callbackUrl) {
    auto pos = callbackUrl.find("state=");
    if (pos == std::string::npos) return "";

    pos += 6;
    auto end = callbackUrl.find('&', pos);
    if (end == std::string::npos) end = callbackUrl.size();
    return callbackUrl.substr(pos, end - pos);
}

// ====== Well-Known ======

OidcWellKnownResult oidcParseWellKnown(const std::string& json) {
    OidcWellKnownResult result;
    result.baseUrl = extractStr(json, "base_url");
    if (result.baseUrl.empty()) result.baseUrl = extractStr(json, "homeserver");
    result.idServer = extractStr(json, "identity_server");

    // OIDC issuer
    auto oidcIssuer = extractStr(json, "org.matrix.msc2965.authentication");
    if (oidcIssuer.empty()) {
        auto authBlock = json;
        auto authPos = authBlock.find("\"org.matrix.msc2965.authentication\"");
        if (authPos != std::string::npos) {
            authPos = authBlock.find('{', authPos);
            if (authPos != std::string::npos) {
                int depth = 0; size_t s = authPos; authPos++;
                while (authPos < authBlock.size()) {
                    if (authBlock[authPos] == '{') depth++;
                    else if (authBlock[authPos] == '}') depth--;
                    if (depth == -1) break;
                    authPos++;
                }
                auto authJson = authBlock.substr(s, authPos - s);
                result.oidcIssuer = extractStr(authJson, "issuer");
            }
        }
    } else {
        result.oidcIssuer = oidcIssuer;
    }

    result.supportsOidc = !result.oidcIssuer.empty();
    result.supportsPassword = json.find("m.login.password") != std::string::npos ||
                              json.find("\"password\"") != std::string::npos;

    return result;
}

bool oidcRequiresOidc(const OidcWellKnownResult& wellKnown) {
    return wellKnown.supportsOidc && !wellKnown.supportsPassword;
}

// ====== Login Flows ======

std::vector<OidcLoginFlow> oidcParseLoginFlows(const std::string& json) {
    std::vector<OidcLoginFlow> flows;
    size_t pos = 0;

    while ((pos = json.find("\"type\"", pos)) != std::string::npos) {
        OidcLoginFlow flow;
        auto typeStr = extractStr(json.substr(pos), "type");
        flow.type = loginTypeFromString(typeStr);

        if (flow.type == OidcLoginType::UNKNOWN) { pos += 5; continue; }

        // Extract identity_providers for SSO
        if (flow.type == OidcLoginType::SSO) {
            auto ipPos = json.find("\"identity_providers\"", pos);
            if (ipPos != std::string::npos && ipPos - pos < 500) {
                ipPos = json.find('[', ipPos);
                if (ipPos != std::string::npos) {
                    ipPos++;
                    while (ipPos < json.size() && json[ipPos] != ']') {
                        if (json[ipPos] == '"') { ipPos++; size_t e = ipPos;
                            while (e < json.size() && json[e] != '"') e++;
                            flow.identityProviders.push_back(json.substr(ipPos, e - ipPos));
                            ipPos = e; }
                        ipPos++;
                    }
                }
            }
        }

        flows.push_back(flow);
        pos += 10;
    }

    return flows;
}

bool oidcHasOidcFlow(const std::vector<OidcLoginFlow>& flows) {
    for (const auto& f : flows) {
        if (f.type == OidcLoginType::OIDC) return true;
    }
    return false;
}

// ====== Password / Token Login ======

std::string buildPasswordLoginRequest(const LoginCredentials& creds) {
    std::ostringstream os;
    os << R"({"type":"m.login.password")";
    os << R"(,"identifier":{"type":"m.id.user","user":")" << creds.userId << R"("})";
    os << R"(,"password":")" << creds.password << R"(")";
    if (!creds.deviceId.empty()) os << R"(,"device_id":")" << creds.deviceId << R"(")";
    if (!creds.initialDeviceDisplayName.empty())
        os << R"(,"initial_device_display_name":")" << creds.initialDeviceDisplayName << R"(")";
    if (creds.refreshToken) os << R"(,"refresh_token":true)";
    os << "}";
    return os.str();
}

std::string buildTokenLoginRequest(const LoginCredentials& creds) {
    std::ostringstream os;
    os << R"({"type":"m.login.token")";
    os << R"(,"token":")" << creds.token << R"(")";
    os << R"(,"txn_id":")" << extractStr(creds.token, "txn") << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
