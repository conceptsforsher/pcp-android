#ifndef PROGRESSIVE_AUTH_UTILS_HPP
#define PROGRESSIVE_AUTH_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Authentication Session Management ----

struct AuthSession {
    std::string sessionId;       // from /login or /register
    std::string type;            // "m.login.password", "m.login.recaptcha", etc.
    std::string params;          // additional params JSON
    bool completed = false;
};

struct AuthFlow {
    std::vector<std::string> stages;  // remaining stages to complete
    std::string sessionId;
    bool completed = false;
    std::string errorMessage;
};

// Parse authentication flow from server response (401 with flows).
AuthFlow parseAuthFlow(const std::string& responseJson, int httpStatus);

// Build authentication request body for a stage.
std::string buildAuthStageBody(const std::string& sessionId, const std::string& type,
    const std::string& paramsJson = "{}");

// Check if a response requires additional authentication.
bool requiresAdditionalAuth(const std::string& responseJson, int httpStatus);

// Parse the session ID from an auth response.
std::string parseAuthSessionId(const std::string& responseJson);

// Check if a specific auth type is supported in the flows.
bool isAuthTypeSupported(const std::string& type, const std::vector<std::string>& flows);

// Get the next required auth stage.
std::string getNextAuthStage(const std::vector<std::string>& completed,
    const std::vector<std::string>& required);

// ---- CAPTCHA / reCAPTCHA ----

struct CaptchaInfo {
    std::string publicKey;
    std::string siteKey;
    std::string server;          // recaptcha server URL
    bool required = false;
};

// Parse captcha info from registration/login response.
CaptchaInfo parseCaptchaInfo(const std::string& responseJson);

// Check if captcha is required for registration.
bool requiresCaptcha(const std::string& responseJson);

// Build captcha response for auth.
std::string buildCaptchaResponse(const std::string& sessionId, const std::string& captchaToken);

// ---- Token-based Auth ----

struct TokenAuth {
    std::string loginToken;
    int64_t expiresInMs = 0;
    bool valid = false;
};

// Parse login token from URL parameters (after SSO redirect).
TokenAuth parseTokenLogin(const std::string& url);

// Build token login request body.
std::string buildTokenLoginBody(const std::string& token, const std::string& deviceName = "");

// Check if a login token has expired.
bool isTokenExpired(const TokenAuth& token);

// ---- Rate Limiting ----

struct RateLimit {
    int retryAfterMs = 0;
    bool isLimited = false;
    std::string errorCode;       // "M_LIMIT_EXCEEDED"
};

// Parse rate limit info from 429 response.
RateLimit parseRateLimit(const std::string& responseJson, int httpStatus);

// Format rate limit message for user display.
std::string formatRateLimitMessage(const RateLimit& limit);

} // namespace progressive

#endif // PROGRESSIVE_AUTH_UTILS_HPP
