#ifndef PROGRESSIVE_IDENTITY_UTILS_HPP
#define PROGRESSIVE_IDENTITY_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Matrix ID Resolution ----

struct ResolvedId {
    std::string input;       // original input
    std::string resolved;    // resolved MXID or room ID
    std::string type;        // "user", "room", "alias", "event", "unknown"
    bool valid = false;
};

// Resolve a Matrix identifier from various input formats.
// Handles: @user:server, #alias:server, !room:server, $event, matrix.to URLs.
ResolvedId resolveMatrixId(const std::string& input);

// ---- 3PID (Third-Party ID) Utilities ----

struct IdentityThreePid {
    std::string medium;     // "email", "msisdn"
    std::string address;    // "user@example.com", "+1234567890"
    bool valid = false;
};

// Parse a 3PID from input.
IdentityThreePid parseThreePid(const std::string& input);

// Check if a string is an email address.
bool isEmail(const std::string& input);

// Check if a string is a phone number (MSISDN).
bool isMsisdn(const std::string& input);

// Format a 3PID for display.
std::string formatThreePid(const IdentityThreePid& pid);

// ---- ThreePID Medium Enum ----
// Original Kotlin: ThreePid.kt sealed class Email/Msisdn, toMedium()
// Original Kotlin: ThirdPartyIdentifier.MEDIUM_EMAIL, MEDIUM_MSISDN

enum class ThreePidMedium {
    EMAIL = 0,       // Original: MEDIUM_EMAIL
    MSISDN = 1,      // Original: MEDIUM_MSISDN
};

const char* threePidMediumToString(ThreePidMedium medium);
ThreePidMedium threePidMediumFromString(const std::string& s);

// ---- ThreePID (Rich Model) ----
// Original Kotlin: ThreePid.kt (Email/Msisdn sealed class) +
//   IdentityPendingBinding.kt (clientSecret, sendAttempt, sid) +
//   IdentityData.kt (identityServerUrl, token, userConsent)

struct ThreePid {
    ThreePidMedium medium = ThreePidMedium::EMAIL;
    std::string address;                    // "user@example.com" or "+1234567890"
    std::string value;                      // alias: same as address (compat with IS_ThreePid)
    int64_t validatedAt = 0;                // ms timestamp when validated
    int64_t addedAt = 0;                    // ms timestamp when added
    bool bound = false;                     // is this 3PID bound to an MXID
    bool valid = false;                     // true if parsed successfully (compat with IS_ThreePid)
    std::string clientSecret;               // UUID for validation session
    std::string sid;                        // session ID from identity server

    // Original: IS_ThreePid::parse() - detect email vs phone from a string
    static ThreePid parse(const std::string& input);

    // Original: IS_ThreePid::isEmail()
    static bool isEmail(const std::string& input);

    // Original: IS_ThreePid::isMsisdn()
    static bool isMsisdn(const std::string& input);
};

// ---- FoundThreePID ----
// Original Kotlin: FoundThreePid.kt (threePid + matrixId)

struct FoundThreePid {
    ThreePid threePid;
    std::string mxid;                       // @user:example.org
};

// ---- IdentityServerError ----
// Original Kotlin: IdentityServiceError.kt
//   OutdatedIdentityServer, OutdatedHomeServer, NoIdentityServerConfigured,
//   TermsNotSignedException, BulkLookupSha256NotSupported,
//   UserConsentNotProvided, BindingError, NoCurrentBindingError

struct IdentityServerError {
    std::string code;                       // e.g. "M_TERMS_NOT_SIGNED", "M_INVALID_PEPPER"
    std::string message;                    // human-readable description
    int httpCode = 0;                       // HTTP status code (401, 403, etc.)
};

// ---- IdentityServiceState ----
// Original Kotlin: IdentityService connection lifecycle (implicit state machine)
// Derived from DefaultIdentityService.kt setNewIdentityServer/disconnect/lookUp/disconnect

enum class IdentityServiceState {
    NOT_CONNECTED = 0,
    CONNECTING = 1,
    CONNECTED = 2,
    DISCONNECTED = 3,
    TOKEN_EXPIRED = 4,
};

const char* identityServiceStateToString(IdentityServiceState state);
IdentityServiceState identityServiceStateFromString(const std::string& s);

// ---- IdentityServerInfo ----
// Original Kotlin: IdentityData.kt
//   (identityServerUrl, token, hashLookupPepper, hashLookupAlgorithm, userConsent)

struct IdentityServerInfo {
    std::string baseUrl;                    // identity server base URL
    std::string accessToken;                // identity server access token
    int64_t tokenExpiry = 0;               // ms timestamp when token expires
    bool isConnected = false;              // current connection state
};

// ---- ThreePidValidationSession ----
// Original Kotlin: IdentityRequestTokenResponse.kt (sid) +
//   IdentityRequestTokenBody.kt (clientSecret, sendAttempt, email/msisdn/country)

struct ThreePidValidationSession {
    std::string sid;                        // server-generated session ID (0-9a-zA-Z.=_- max 255)
    std::string clientSecret;               // client-generated secret (UUID)
    std::string nextLink;                   // optional deep link for validation
    std::string submitUrl;                  // URL to submit validation token
    std::string medium;                     // "email" or "msisdn"
    std::string address;                    // target address
    int64_t validatedAt = 0;               // ms timestamp
};

// ---- JSON Builders ----
// Original Kotlin: IdentityRequestTokenForEmailBody.kt,
//   IdentityRequestTokenForMsisdnBody.kt,
//   IdentityRequestOwnershipParams.kt,
//   IdentityLookUpParams.kt (unhashed variant)

// Build request body for requesting a validation token (email or MSISDN).
// POST /_matrix/identity/v2/validate/email/requestToken
//      /_matrix/identity/v2/validate/msisdn/requestToken
// For email: {"client_secret":"...","send_attempt":1,"email":"..."}
// For MSISDN: {"client_secret":"...","send_attempt":1,"phone_number":"...","country":"..."}
std::string buildThreePidRequest(const ThreePid& pid,
                                 const std::string& clientSecret,
                                 int sendAttempt,
                                 const std::string& countryCode = "");

// Build request body for submitting a validation token.
// POST /_matrix/identity/v2/validate/{medium}/submitToken
// {"client_secret":"...","sid":"...","token":"..."}
std::string buildThreePidValidationRequest(const std::string& sid,
                                           const std::string& clientSecret,
                                           const std::string& token);

// Build bulk lookup request body (unhashed "threepids" format).
// POST /_matrix/identity/v2/lookup  [legacy/threepid lookup]
// {"threepids":[["medium","address"],...]}
std::string buildBulkLookupBody(const std::vector<ThreePid>& threepids);

// ---- Manual Parsers ----
// Original Kotlin: IdentityRequestTokenResponse.kt,
//   IdentityBulkLookupTask.kt handleSuccess(),
//   IdentityLookUpResponse.kt (mappings)

// Parse a token request response: {"sid":"..."}
ThreePidValidationSession parseThreePidTokenResponse(const std::string& json);

// Parse a bulk lookup response with mappings: {"mappings":{"hashedAddress":"@mxid",...}}
// originalPids is used to map hashed addresses back to ThreePid objects.
std::vector<FoundThreePid> parseBulkLookupResponse(
    const std::string& json,
    const std::vector<ThreePid>& originalPids);

// ---- Display Name Utilities ----

// Check if two display names are ambiguous (same or very similar).
bool isAmbiguousName(const std::string& name1, const std::string& name2);

// Generate a unique display name if duplicates exist.
// "Alice" + duplicates -> "Alice (@alice:matrix.org)"
std::string disambiguateName(const std::string& displayName, const std::string& mxid);

// Check if a display name violates Matrix rules.
bool isValidDisplayName(const std::string& name);

// Get initials from a display name: "Alice Johnson" -> "AJ"
std::string getIdentityInitials(const std::string& displayName, int maxChars = 2);

// ---- Room Alias Utilities ----

// Check if an alias is canonical (matches expected format).
bool isCanonicalAlias(const std::string& alias, const std::string& expectedRoomId);

// Extract the local part of an alias: "#room:server" -> "room"
std::string extractAliasLocalpart(const std::string& alias);

// Suggest room aliases from a room name.
std::vector<std::string> suggestAliases(const std::string& roomName, int maxResults = 3);

} // namespace progressive

#endif // PROGRESSIVE_IDENTITY_UTILS_HPP
