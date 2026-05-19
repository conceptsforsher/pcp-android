#ifndef PROGRESSIVE_ACCOUNT_UTILS_HPP
#define PROGRESSIVE_ACCOUNT_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Account Management Utilities ----

struct AccountInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string homeServer;
    std::string deviceId;
    int64_t createdAtMs = 0;
    bool isAdmin = false;
    bool isDeactivated = false;
};

struct PasswordChange {
    std::string oldPassword;
    std::string newPassword;
    bool logoutOtherDevices = true;
};

struct PasswordValidation {
    bool valid = false;
    std::string errorMessage;
    bool tooShort = false;
    bool noUpperCase = false;
    bool noLowerCase = false;
    bool noDigit = false;
    bool noSpecialChar = false;
    bool matchesUsername = false;
};

// Validate a new password against best practices.
PasswordValidation validateAccountPassword(const std::string& password,
    const std::string& username = "", int minLength = 8);

// Build password change request body JSON.
std::string buildPasswordChangeBody(const PasswordChange& change, const std::string& authSession = "");

// Build deactivate account request body.
std::string buildDeactivateBody(const std::string& authSession = "");

// Parse account info from /account/whoami response.
AccountInfo parseAccountInfo(const std::string& apiResponseJson);

// Format account info for display.
std::string formatAccountInfo(const AccountInfo& info);

// Check if a display name is valid (1-100 chars, not only whitespace).
bool isValidDisplayName(const std::string& name, int maxLen = 100);

// Check if avatar URL is an MXC URI.
bool isValidAvatarUrl(const std::string& url);

// ---- Email / Phone Management ----

struct ThreePidRequest {
    std::string medium;      // "email" or "msisdn"
    std::string address;     // "user@example.com" or "+1234567890"
    std::string clientSecret;
    int sendAttempt = 1;     // which attempt number
    std::string idServer;    // identity server URL
    std::string homeServer;
};

struct ThreePidValidation {
    bool valid = false;
    std::string medium;
    std::string address;
    std::string errorMessage;
    bool invalidFormat = false;
    bool alreadyUsed = false;
    bool serverUnavailable = false;
};

// Validate an email or phone number for adding to account.
ThreePidValidation validateThreePid(const std::string& input, bool isEmail);

// Build the request body for adding a 3PID to account.
std::string buildThreePidRequestBody(const ThreePidRequest& req);

// Build the request body for binding a 3PID after verification.
std::string buildThreePidBindBody(const std::string& clientSecret, const std::string& sid,
    const std::string& idServer = "");

// Parse the email/phone request token response.
std::string parseThreePidSid(const std::string& responseJson);

} // namespace progressive

#endif // PROGRESSIVE_ACCOUNT_UTILS_HPP
