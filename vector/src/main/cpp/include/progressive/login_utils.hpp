#ifndef PROGRESSIVE_LOGIN_UTILS_HPP
#define PROGRESSIVE_LOGIN_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// Login type — from AuthLoginType.kt (40L)
enum class AuthLoginType { Password, Sso, Unsupported, Custom, Direct, Unknown, Qr };

// ---- Registration Flow (from RegistrationFlowResponse.kt 115L) ----
struct RegistrationStage {
    std::string type;        // "m.login.recaptcha", "m.login.dummy", etc.
    bool isMandatory = false;
    std::string publicKey;   // reCAPTCHA public key
    bool isEmail = false;
    bool isMsisdn = false;
};

struct RegistrationFlowResult {
    std::vector<RegistrationStage> missingStages;    // still need to complete
    std::vector<RegistrationStage> completedStages;  // already done
    std::string session;
    bool hasStages = false;
};

// Parse registration flow response into stages.
RegistrationFlowResult parseRegistrationFlow(const std::string& json);

// ---- Login Flow Utilities ----

struct LoginFlow {
    std::string type;              // "m.login.password", "m.login.token", "m.login.sso"
    std::string description;       // human-readable
    bool isSupported = true;
    std::vector<std::string> stages; // for multi-stage auth
};

struct LoginParams {
    std::string userId;
    std::string password;
    std::string token;
    std::string deviceName;
    std::string deviceId;
    std::string homeServer;
    bool storeCredentials = true;
    bool refreshToken = false;
};

struct LoginResult {
    bool success = false;
    std::string userId;
    std::string accessToken;
    std::string refreshToken;
    std::string deviceId;
    std::string homeServer;
    std::string errorMessage;
    std::string errorCode;
    int httpStatus = 0;
};

// Parse available login flows from /login response.
std::vector<LoginFlow> parseLoginFlowsList(const std::string& apiResponseJson);

// Build the login request body JSON.
std::string buildLoginBody(const LoginParams& params);

// Build a Matrix identifier object for the login body.
std::string buildUserIdentifier(const std::string& userId);

// Validate a username/password for basic login.
bool isValidLoginCredentials(const std::string& userId, const std::string& password);

// Parse the login response to extract credentials.
LoginResult parseLoginResponse(const std::string& responseJson, int httpStatus);

// Build a refresh token request body.
std::string buildRefreshBody(const std::string& refreshToken);

// Check if a response indicates rate limiting.
bool isRateLimited(const std::string& responseJson, int httpStatus);

// Get retry delay from rate limit response (seconds).
int getRateLimitRetrySeconds(const std::string& responseJson);

// ---- Device Name Generator ----

// Generate a device name from device info.
std::string generateDeviceName(const std::string& model, const std::string& osVersion);

// Generate a unique device ID.
std::string generateDeviceId();

// Parse device name from a user agent string.
std::string parseDeviceName(const std::string& userAgent);

// ---- Well-Known Discovery ----

struct WellKnownResult {
    std::string homeServerBaseUrl;
    std::string identityServerBaseUrl;
    bool valid = false;
    bool isWellKnown = false;
    std::string errorMessage;
    // JNI compat convenience fields
    std::string baseUrl;         // alias for homeServerBaseUrl
    std::string oidcIssuer;
    bool supportsOidc = false;
    bool supportsPassword = true;
};

// Parse /.well-known/matrix/client response.
WellKnownResult parseWellKnown(const std::string& responseJson);

// Check if a homeserver URL needs well-known discovery.
bool needsWellKnownDiscovery(const std::string& homeserverUrl);

// Build well-known discovery URL.
std::string buildWellKnownUrl(const std::string& domain);

// Check if well-known result requires OIDC authentication.
bool requiresOidc(const WellKnownResult& wk);

} // namespace progressive

#endif // PROGRESSIVE_LOGIN_UTILS_HPP
