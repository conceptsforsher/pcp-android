#include "progressive/auth_models.hpp"
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Identity Server Manager — 3PID management, identity server API
//
// Faithful port from Element Android original sources:
//   IS_ThreePid.kt — sealed Email/Msisdn, toMedium(), getCountryCode()
//   IdentityService.kt — get/set identity server, bind/unbind 3PID,
//     lookUp, user consent, getShareStatus, sign3pidInvitation
//   IS_FoundThreePid.kt — threePid + matrixId
//   IS_SharedState.kt — SHARED, NOT_SHARED, BINDING_IN_PROGRESS
//
// Covers:
//   1. ThreePID type (Email, MSISDN/phone)
//   2. Identity server discovery and configuration
//   3. 3PID bind/unbind lifecycle
//   4. 3PID lookup (email→MatrixID)
//   5. User consent management
//   6. Share status tracking
//   7. Identity server validation
//   8. Invitation signing
// ================================================================

// ---- ThreePID Type ----
// Original: IS_ThreePid.kt sealed Email/Msisdn

enum class ThreePidMedium {
    EMAIL = 0,       // Original: MEDIUM_EMAIL
    MSISDN = 1,      // Original: MEDIUM_MSISDN (phone number)
};

const char* threePidMediumToString(ThreePidMedium medium);
ThreePidMedium threePidMediumFromString(const std::string& s);

// ---- ThreePID ----
// Original: IS_ThreePid.kt (Email(email), Msisdn(msisdn))

struct IS_ThreePid {
    ThreePidMedium medium = ThreePidMedium::EMAIL;
    std::string value;               // "alice@example.org" or "1234567890"
    bool valid = false;

    // Original: toMedium()
    std::string toMedium() const;

    // Original: getCountryCode() for MSISDN
    std::string getCountryCode() const;

    // Parse a threePID from a string (detects email vs phone).
    static IS_ThreePid parse(const std::string& input);

    // Check if input is an email.
    static bool isEmail(const std::string& input);

    // Check if input is a phone number (MSISDN).
    static bool isMsisdn(const std::string& input);
};

// ---- Found ThreePID ----
// Original: IS_FoundThreePid.kt (threePid, matrixId)

struct IS_FoundThreePid {
    IS_ThreePid threePid;
    std::string matrixId;            // @user:example.org
    bool valid = false;
};

// ---- Share State ----
// Original: IS_SharedState.kt (SHARED, NOT_SHARED, BINDING_IN_PROGRESS)

enum class IS_SharedState {
    SHARED = 0,              // 3PID is shared with identity server
    NOT_SHARED = 1,          // Not shared
    BINDING_IN_PROGRESS = 2, // Binding confirmation pending
};

const char* sharedStateToString(IS_SharedState state);
IS_SharedState sharedStateFromString(const std::string& s);

// ---- 3PID Binding Status ----

struct IS_ThreePidBindingStatus {
    IS_ThreePid threePid;
    IS_SharedState shareState = IS_SharedState::NOT_SHARED;
    std::string sid;                 // Session ID for pending binding
    bool isBound = false;            // Successfully bound
    int64_t boundAtMs = 0;
    std::string errorMessage;
};

// ---- Sign Invitation Result ----
// Original: SignInvitationResult

struct SignInvitationResult {
    std::string mxid;                // Matrix ID
    std::string token;
    std::string signatures;
    bool valid = false;
};

// ---- Identity Server Config ----

struct IdentityServerConfig {
    std::string defaultServerUrl;    // From login/well-known
    std::string currentServerUrl;    // Currently configured
    bool isValid = false;            // Server is reachable and valid
    bool userConsent = false;        // User consented to data sharing
};

// ---- Identity Server Manager ----

class IdentityServerManager {
public:
    IdentityServerManager();

    // ====== Server Configuration ======
    // Original: IdentityService.getDefaultIdentityServer / getCurrentIdentityServerUrl

    // Set the default identity server (from login/well-known).
    void setDefaultServer(const std::string& url);

    // Set the current identity server URL.
    void setCurrentServer(const std::string& url);

    // Get the current server URL.
    std::string getCurrentServerUrl() const;

    // Get the default server URL.
    std::string getDefaultServerUrl() const;

    // Original: setNewIdentityServer(url) → final url
    // Prepends "https://" if needed.
    std::string setNewIdentityServer(const std::string& url, std::string& error);

    // Original: disconnect() — logout from identity server
    void disconnect();

    // ====== Server Validation ======
    // Original: isValidIdentityServer(url)

    // Build the identity server status check URL.
    std::string buildStatusCheckUrl(const std::string& serverUrl) const;

    // Parse identity server status response.
    bool parseStatusResponse(const std::string& json) const;

    // Check if a URL is a valid-looking identity server URL.
    bool isValidServerUrl(const std::string& url) const;

    // ====== ThreePID Management ======
    // Original: startBindThreePid / cancelBindThreePid / finalizeBindThreePid / unbindThreePid

    // Build bind request body.
    std::string buildBindRequest(const IS_ThreePid& threePid) const;

    // Build unbind request body.
    std::string buildUnbindRequest(const IS_ThreePid& threePid) const;

    // Build validation token submission request.
    std::string buildSubmitTokenRequest(const IS_ThreePid& threePid, const std::string& sid,
                                         const std::string& clientSecret, int token) const;

    // Parse bind response.
    IS_ThreePidBindingStatus parseBindResponse(const std::string& json, const IS_ThreePid& threePid) const;

    // Register a binding session.
    void registerBinding(const std::string& sid, const IS_ThreePid& threePid);

    // Get binding by session ID.
    IS_ThreePidBindingStatus getBinding(const std::string& sid) const;

    // Cancel a pending binding.
    void cancelBinding(const std::string& sid);

    // Finalize a binding.
    void finalizeBinding(const std::string& sid);

    // Remove a binding.
    void removeBinding(const std::string& sid);

    // ====== 3PID Lookup ======
    // Original: lookUp(threePids) → List<IS_FoundThreePid>

    // Build lookup request for multiple 3PIDs.
    std::string buildLookupRequest(const std::vector<IS_ThreePid>& threePids) const;

    // Parse lookup response.
    std::vector<IS_FoundThreePid> parseLookupResponse(const std::string& json) const;

    // ====== User Consent ======
    // Original: getUserConsent / setUserConsent

    void setUserConsent(bool consent);
    bool getUserConsent() const;

    // Build consent request body.
    std::string buildConsentRequest(bool consent) const;

    // ====== Share Status ======
    // Original: getShareStatus(threePids) → Map<IS_ThreePid, IS_SharedState>

    // Get share status for 3PIDs.
    IS_SharedState getShareStatus(const IS_ThreePid& threePid) const;

    // Set share status.
    void setShareStatus(const IS_ThreePid& threePid, IS_SharedState state);

    // ====== Invitation Signing ======
    // Original: sign3pidInvitation(identityServer, token, secret)

    // Build invitation signing request.
    std::string buildSignInvitationRequest(const std::string& token, const std::string& secret) const;

    // Parse signing response.
    SignInvitationResult parseSignInvitationResponse(const std::string& json) const;

    // ====== Serialization ======

    // Export 3PID as JSON.
    std::string threePidToJson(const IS_ThreePid& threePid) const;

    // Export binding status as JSON.
    std::string bindingToJson(const IS_ThreePidBindingStatus& status) const;

    // Export found 3PID as JSON.
    std::string foundPidToJson(const IS_FoundThreePid& found) const;

private:
    IdentityServerConfig config_;
    std::unordered_map<std::string, IS_ThreePidBindingStatus> bindings_; // sid → binding

    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

} // namespace progressive
