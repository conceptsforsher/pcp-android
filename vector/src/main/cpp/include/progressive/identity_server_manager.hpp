#pragma once

#include "progressive/auth_models.hpp"
#include "progressive/identity_utils.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Identity Server Manager - 3PID management, identity server API
//
// Faithful port from Element Android original sources:
//   IS_ThreePid.kt - sealed Email/Msisdn, toMedium(), getCountryCode()
//   IdentityService.kt - get/set identity server, bind/unbind 3PID,
//     lookUp, user consent, getShareStatus, sign3pidInvitation
//   IS_FoundThreePid.kt - threePid + matrixId
//   IS_SharedState.kt - SHARED, NOT_SHARED, BINDING_IN_PROGRESS
//   DefaultIdentityService.kt - full implementation
//   IdentityBulkLookupTask.kt - bulk lookup with SHA-256 hashing
//   IdentityRegisterTask.kt - OpenID token exchange
//   IdentityPingTask.kt - IS availability check
//   IdentityDisconnectTask.kt - logout
//   IdentityRequestTokenForBindingTask.kt - start binding
//
// Covers:
//   1. ThreePID type (Email, MSISDN/phone)
//   2. Identity server discovery and configuration
//   3. 3PID bind/unbind lifecycle
//   4. 3PID lookup (email->MatrixID)
//   5. User consent management
//   6. Share status tracking
//   7. Identity server validation (ping)
//   8. Invitation signing
//   9. Privacy-preserving hashed lookup (SHA-256)
//  10. Bulk lookup request/response
// ================================================================

// ThreePidMedium enum and helpers now in identity_utils.hpp
// (threePidMediumToString, threePidMediumFromString)

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
    std::string token;               // Invitation token
    std::string signatures;          // Signed data
    bool valid = false;
};

// ---- Identity Server Config ----

struct IdentityServerConfig {
    std::string defaultServerUrl;    // From login/well-known
    std::string currentServerUrl;    // Currently configured
    bool isValid = false;            // Server is reachable and valid
    bool userConsent = false;        // User consented to data sharing
};

// ---- NEW: Identity Bulk Lookup Request/Response ----
// Original Kotlin: IdentityBulkLookupTask.kt Params(threepids),
//   IdentityLookUpParams.kt, IdentityLookUpResponse.kt

struct IdentityBulkLookupRequest {
    std::vector<ThreePid> threepids;        // 3PIDs to look up
};

struct IdentityBulkLookupResponse {
    std::vector<FoundThreePid> threepids;   // resolved 3PIDs with MXIDs
    bool limited = false;                   // server returned partial results
};

// ---- NEW: Identity Register Request/Response ----
// Original Kotlin: IdentityRegisterTask.kt, IdentityRegisterResponse.kt,
//   IdentityAuthAPI.kt register() - exchanges OpenID token for IS token
// POST /_matrix/identity/v2/account/register

struct IdentityRegisterRequest {
    std::string address;                    // email/MSISDN address (unused in actual register, kept for parity)
    std::string clientSecret;               // unused in register flow
    int sendAttempt = 0;                    // unused in register flow
    std::string nextLink;                   // unused in register flow
    std::string idAccessToken;              // OpenID access_token from homeserver
};

struct IdentityRegisterResponse {
    // Original Kotlin: IdentityRegisterResponse(token) - the registered token
    std::string sid;                        // identity server access token (named "token" in spec)
};

// ---- NEW: Ping Response ----
// Original Kotlin: IdentityPingTask.kt, IdentityAuthAPI.kt ping()

struct IdentityPingResponse {
    std::string status;                     // "ok" on success, error description otherwise
};

// ---- NEW: Disconnect Response ----
// Original Kotlin: IdentityDisconnectTask.kt, IdentityAPI.kt logout()

struct IdentityDisconnectResponse {
    std::string status;                     // "ok" on success
};

// ---- NEW: Token Response ----
// Original Kotlin: IdentityRegisterResponse.kt, EnsureIdentityToken.kt

struct IdentityTokenResponse {
    std::string token;                      // identity server access token
    int64_t ttl = 0;                        // time-to-live in seconds
};

// ---- NEW: Identity Binding Response ----
// Original Kotlin: identity binding API - 3PID association info

struct IdentityBindingResponse {
    std::string address;                    // 3PID address (email or phone)
    std::string medium;                     // "email" or "msisdn"
    std::string mxid;                       // bound Matrix ID
    int64_t notBefore = 0;                 // ms timestamp - binding start
    int64_t notAfter = 0;                  // ms timestamp - binding expiry
    int64_t ts = 0;                        // ms timestamp - server time
};

// ---- NEW: ThreePidCredentials ----
// Original Kotlin: IdentityPendingBinding.kt (clientSecret, sendAttempt, sid)

struct ThreePidCredentials {
    std::string sid;                        // session ID from identity server
    std::string clientSecret;               // client-generated secret (UUID)
    std::string idServer;                   // identity server base URL
    std::string idAccessToken;              // identity server access token
};

// ---- Identity Server Manager ----

class IdentityServerManager {
public:
    IdentityServerManager();

    // ====== Server Configuration ======
    // Original: IdentityService.getDefaultIdentityServer / getCurrentIdentityServerUrl

    void setDefaultServer(const std::string& url);
    void setCurrentServer(const std::string& url);
    std::string getCurrentServerUrl() const;
    std::string getDefaultServerUrl() const;

    // Original: setNewIdentityServer(url) -> final url
    std::string setNewIdentityServer(const std::string& url, std::string& error);

    // Original: disconnect() - logout from identity server
    void disconnect();

    // ====== Server Validation ======
    // Original: isValidIdentityServer(url)

    std::string buildStatusCheckUrl(const std::string& serverUrl) const;
    bool parseStatusResponse(const std::string& json) const;
    bool isValidServerUrl(const std::string& url) const;

    // ====== ThreePID Management ======
    // Original: startBindThreePid / cancelBindThreePid / finalizeBindThreePid / unbindThreePid

    std::string buildBindRequest(const IS_ThreePid& threePid) const;
    std::string buildUnbindRequest(const IS_ThreePid& threePid) const;
    std::string buildSubmitTokenRequest(const IS_ThreePid& threePid, const std::string& sid,
                                         const std::string& clientSecret, int token) const;
    IS_ThreePidBindingStatus parseBindResponse(const std::string& json, const IS_ThreePid& threePid) const;

    void registerBinding(const std::string& sid, const IS_ThreePid& threePid);
    IS_ThreePidBindingStatus getBinding(const std::string& sid) const;
    void cancelBinding(const std::string& sid);
    void finalizeBinding(const std::string& sid);
    void removeBinding(const std::string& sid);

    // ====== 3PID Lookup ======
    // Original: lookUp(threePids) -> List<IS_FoundThreePid>

    std::string buildLookupRequest(const std::vector<IS_ThreePid>& threePids) const;
    std::vector<IS_FoundThreePid> parseLookupResponse(const std::string& json) const;

    // ====== NEW: Generic Identity API Request Builder ======
    // Original Kotlin: IdentityAPI.kt, IdentityAuthAPI.kt - Retrofit-based REST calls
    //
    // Build a generic JSON request envelope with optional Authorization header token.
    // Returns the JSON body string; the auth token is meant to be set as a header by the caller.
    std::string buildIdentityApiRequest(const std::string& endpoint,
                                        const std::string& body,
                                        const std::string& accessToken = "") const;

    // ====== NEW: Bulk Lookup (Hashed and Unhashed) ======
    // Original Kotlin: IdentityBulkLookupTask.kt

    // Build bulk lookup body with unhashed "threepids" format (legacy).
    std::string buildBulkLookupBody(const std::vector<ThreePid>& threepids) const;

    // Build hashed lookup request body (privacy-preserving).
    // Format: {"addresses":[...], "algorithm":"sha256", "pepper":"..."}
    std::string buildHashedLookupRequest(const std::vector<std::string>& hashedAddresses,
                                         const std::string& algorithm,
                                         const std::string& pepper) const;

    // Parse bulk lookup response (hashed mappings -> FoundThreePid list).
    IdentityBulkLookupResponse parseBulkLookupResponse(const std::string& json,
                                                        const std::vector<ThreePid>& originalPids) const;

    // ====== NEW: Register ======
    // Original Kotlin: IdentityRegisterTask.kt, IdentityRegisterResponse.kt

    // Build register request body (OpenID token exchange for IS token).
    std::string buildRegisterBody(const IdentityRegisterRequest& req) const;

    // Parse register response {"token":"..."}.
    IdentityRegisterResponse parseRegisterResponse(const std::string& json) const;

    // ====== NEW: Binding Status ======
    // Original Kotlin: identity binding API endpoint

    // Parse identity binding response.
    IdentityBindingResponse parseIdentityBindingResponse(const std::string& json) const;

    // ====== User Consent ======
    // Original: getUserConsent / setUserConsent

    void setUserConsent(bool consent);
    bool getUserConsent() const;
    std::string buildConsentRequest(bool consent) const;

    // ====== Share Status ======
    // Original: getShareStatus(threePids) -> Map<IS_ThreePid, IS_SharedState>

    IS_SharedState getShareStatus(const IS_ThreePid& threePid) const;
    void setShareStatus(const IS_ThreePid& threePid, IS_SharedState state);

    // ====== Invitation Signing ======
    // Original: sign3pidInvitation(identityServer, token, secret)

    std::string buildSignInvitationRequest(const std::string& token, const std::string& secret) const;
    SignInvitationResult parseSignInvitationResponse(const std::string& json) const;

    // ====== Serialization ======

    std::string threePidToJson(const IS_ThreePid& threePid) const;
    std::string bindingToJson(const IS_ThreePidBindingStatus& status) const;
    std::string foundPidToJson(const IS_FoundThreePid& found) const;

private:
    IdentityServerConfig config_;
    std::unordered_map<std::string, IS_ThreePidBindingStatus> bindings_; // sid -> binding

    static std::string extractStr(const std::string& json, const std::string& key);
    static int64_t extractInt(const std::string& json, const std::string& key);
    static bool extractBool(const std::string& json, const std::string& key);
};

// ====== Free Functions ======

// ---- Identity Server Availability ----
// Original Kotlin: IdentityPingTask.kt, IdentityAuthAPI.kt ping()
//
// Check if the identity server at the given URL is reachable and returning valid v2 API.
// Returns true if the server responds with a JSON body containing a "version" key.
bool isIdentityServerAvailable(const std::string& identityServerUrl);

// ---- Display Formatting ----
// Original Kotlin: ThreePid.kt display formatting
//
// Format a 3PID for display: "email@example.com" or "+1 234 567 8901".
std::string formatThreePidForDisplay(const ThreePid& pid);

// ---- Binding Check ----
// Original Kotlin: IdentityService.kt getShareStatus() - checks lookup results
//
// Check if a 3PID is bound to the given MXID by querying the identity server.
// Returns true if the 3PID resolves to the given MXID.
bool isThreePidBound(const ThreePid& pid, const std::string& mxid);

// ---- Hash Lookup ----
// Original Kotlin: Sha256Converter.kt, IdentityBulkLookupTask.kt getHashedAddresses()
//
// Compute a SHA-256 hash for a 3PID address using identity server pepper.
// Format: SHA-256(lowercase(address) + " " + medium + " " + pepper)
// Returns the hash as a raw 32-byte string (caller should base64url-encode).
std::string hashThreePidAddress(const std::string& address,
                                const std::string& medium,
                                const std::string& pepper);

// Build a hashed bulk lookup request JSON body.
// Uses the given algorithm (e.g. "sha256") and pepper.
// {"addresses":["hashed1","hashed2",...],"algorithm":"sha256","pepper":"..."}
std::string buildHashedLookupRequest(const std::vector<std::string>& hashedAddresses,
                                     const std::string& algorithm,
                                     const std::string& pepper);

} // namespace progressive
