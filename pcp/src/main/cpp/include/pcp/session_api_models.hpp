#pragma once
// ============================================================================
// session_api_models.hpp — Auth, Session, Space, Identity API models
//
// Ported from:
//   matrix-sdk-android/src/main/java/org/matrix/android/sdk/api/auth/
//   matrix-sdk-android/src/main/java/org/matrix/android/sdk/api/session/space/
//   matrix-sdk-android/src/main/java/org/matrix/android/sdk/api/session/identity/
// ============================================================================

#include <string>
#include <vector>
#include <cstdint>

namespace pcp {
namespace session_models {

// ==== LoginType (auth/LoginType.kt:19-40) ====
enum class LoginType { PASSWORD, SSO, UNSUPPORTED, CUSTOM, DIRECT, UNKNOWN, QR };
inline LoginType loginTypeFromName(const std::string& name) {
    if (name == "PASSWORD") return LoginType::PASSWORD;
    if (name == "SSO") return LoginType::SSO;
    if (name == "UNSUPPORTED") return LoginType::UNSUPPORTED;
    if (name == "CUSTOM") return LoginType::CUSTOM;
    if (name == "DIRECT") return LoginType::DIRECT;
    if (name == "QR") return LoginType::QR;
    return LoginType::UNKNOWN;
}
inline const char* loginTypeToString(LoginType t) {
    switch (t) {
        case LoginType::PASSWORD: return "PASSWORD";
        case LoginType::SSO: return "SSO";
        case LoginType::UNSUPPORTED: return "UNSUPPORTED";
        case LoginType::CUSTOM: return "CUSTOM";
        case LoginType::DIRECT: return "DIRECT";
        case LoginType::QR: return "QR";
        default: return "UNKNOWN";
    }
}

// ==== LoginFlowTypes (auth/data/LoginFlowTypes.kt:19-31) ====
namespace LoginFlowTypes {
    constexpr const char* PASSWORD = "m.login.password";
    constexpr const char* OAUTH2 = "m.login.oauth2";
    constexpr const char* EMAIL_CODE = "m.login.email.code";
    constexpr const char* EMAIL_URL = "m.login.email.url";
    constexpr const char* EMAIL_IDENTITY = "m.login.email.identity";
    constexpr const char* MSISDN = "m.login.msisdn";
    constexpr const char* RECAPTCHA = "m.login.recaptcha";
    constexpr const char* DUMMY = "m.login.dummy";
    constexpr const char* TERMS = "m.login.terms";
    constexpr const char* TOKEN = "m.login.token";
    constexpr const char* SSO = "m.login.sso";
}

// ==== DiscoveryInformation / WellKnownBaseConfig (auth/data/) ====
//
// Original Kotlin:
//   data class DiscoveryInformation(@Json(name="m.homeserver") homeServer:
//       WellKnownBaseConfig? = null, @Json(name="m.identity_server") identityServer:
//       WellKnownBaseConfig? = null)
struct WellKnownBaseConfig {
    std::string baseUrl;
};

struct DiscoveryInformation {
    WellKnownBaseConfig homeServer;
    bool hasHomeServer = false;
    WellKnownBaseConfig identityServer;
    bool hasIdentityServer = false;
};

// ==== Credentials (auth/data/Credentials.kt:30-59) ====
//
// Original Kotlin:
//   data class Credentials(@Json(name="user_id") val userId: String,
//       @Json(name="access_token") val accessToken: String,
//       @Json(name="refresh_token") val refreshToken: String?,
//       @Json(name="home_server") val homeServer: String?,
//       @Json(name="device_id") val deviceId: String,
//       @Json(name="well_known") val discoveryInformation:
//           DiscoveryInformation? = null)
struct Credentials {
    std::string userId;
    std::string accessToken;
    std::string refreshToken;
    std::string homeServer;          // deprecated
    std::string deviceId;
    DiscoveryInformation discoveryInformation;
    bool hasDiscoveryInfo = false;

    // Original Kotlin: fun sessionId(): String = md5("$userId|$deviceId")
    // Computed on demand via hash_utils
    bool isLoggedIn() const { return !accessToken.empty(); }
};

// ==== HomeServerConnectionConfig (auth/data/) ====
struct HomeServerConnectionConfig {
    std::string homeServerUri;       // entered by user
    std::string homeServerUriBase;   // after possible redirect
    std::string homeServerHost;      // extracted from homeServerUri
    std::string identityServerUri;   // default identity server

    bool hasIdentityServer() const { return !identityServerUri.empty(); }
};

// ==== SessionParams (auth/data/SessionParams.kt:25-80) ====
struct SessionParams {
    Credentials credentials;
    HomeServerConnectionConfig homeServerConnectionConfig;
    bool isTokenValid = true;
    LoginType loginType = LoginType::UNKNOWN;

    // Shortcuts (Original Kotlin computed properties)
    std::string getUserId() const { return credentials.userId; }
    std::string getDeviceId() const { return credentials.deviceId; }
    std::string getHomeServerUrl() const { return homeServerConnectionConfig.homeServerUri; }
    std::string getHomeServerUrlBase() const { return homeServerConnectionConfig.homeServerUriBase; }
    std::string getHomeServerHost() const { return homeServerConnectionConfig.homeServerHost; }
    std::string getDefaultIdentityServerUrl() const { return homeServerConnectionConfig.identityServerUri; }
    bool isOpen() const { return isTokenValid && !credentials.accessToken.empty(); }
};

// ==== WellKnown (auth/data/WellKnown.kt:49-70) ====
struct DelegatedAuthConfig {
    std::string issuer;
    std::string authorizationEndpoint;
    std::string tokenEndpoint;
    std::string registrationEndpoint;
};

struct WellKnown {
    WellKnownBaseConfig homeServer;
    bool hasHomeServer = false;
    WellKnownBaseConfig identityServer;
    bool hasIdentityServer = false;
    std::string integrationsJson;     // raw JSON dict
    DelegatedAuthConfig delegatedAuthConfig;
    bool hasDelegatedAuth = false;
    bool disableNetworkConstraint = false;
};

// ==== SsoIdentityProvider (auth/data/) ====
struct SsoIdentityProvider {
    std::string id;
    std::string name;
    std::string icon;
    std::string brand;
};

// ==== LoginFlowResult (auth/data/) ====
struct LoginFlowResult {
    std::string type;                // e.g. "m.login.password"
    std::vector<std::string> stages; // for multi-stage auth
    std::string paramsJson;          // raw JSON
};

// ==== RegistrationResult / RegistrationAvailability ====
struct RegistrationResult {
    std::string userId;
    std::string accessToken;
    std::string deviceId;
    std::string homeServer;
};

struct RegistrationAvailability {
    bool available = false;
};

struct RegisterThreePid {
    std::string medium;              // "email" or "msisdn"
    std::string address;
    std::string clientSecret;
    std::string sid;
};

// ==== Stage (auth/registration/) ====
enum class RegistrationStage { RESPONSE, SUCCESS_DONE, UNKNOWN };
inline RegistrationStage stageFromString(const std::string& s) {
    if (s == "m.login.terms") return RegistrationStage::RESPONSE;
    if (s == "m.login.dummy") return RegistrationStage::SUCCESS_DONE;
    if (s == "m.login.recaptcha") return RegistrationStage::RESPONSE;
    if (s == "m.login.email.identity") return RegistrationStage::RESPONSE;
    if (s == "m.login.msisdn") return RegistrationStage::RESPONSE;
    if (s == "m.login.registration_token") return RegistrationStage::RESPONSE;
    return RegistrationStage::UNKNOWN;
}

// ==== UIABaseAuth / UserPasswordAuth / TokenBasedAuth ====
enum class AuthType { PASSWORD, SSO, TOKEN, UNKNOWN };

struct UIAuthParams {
    AuthType type = AuthType::UNKNOWN;
    std::string session;             // UIA session ID
    std::string user;
    std::string password;
    std::string token;
    std::string tokenLoginType;
    std::string ssoRedirectUrl;
};

// ==== Space Models ====

// SpaceChildContent (session/space/model/SpaceChildContent.kt:33-73)
struct SpaceChildContent {
    std::vector<std::string> via;
    std::string order;
    bool suggested = false;

    bool validOrder() const {
        if (order.empty() || order.size() > 50) return false;
        for (char c : order) {
            if (c < 0x20 || c > 0x7E) return false;
        }
        return true;
    }
};

// SpaceParentContent (session/space/model/SpaceParentContent.kt:34-48)
struct SpaceParentContent {
    std::vector<std::string> via;
    bool canonical = false;
};

// SpaceOrderContent
struct SpaceOrderContent {
    std::string order;
    bool validOrder() const {
        if (order.empty() || order.size() > 50) return false;
        for (char c : order) {
            if (c < 0x20 || c > 0x7E) return false;
        }
        return true;
    }
};

// SpaceChildSummaryEvent (session/space/model/)
struct SpaceChildSummaryEvent {
    std::string roomId;
    std::string viaJson;             // raw JSON array
    std::string order;
    bool suggested = false;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    int activeMemberCount = 0;
    std::string roomType;
};

// CreateSpaceParams (session/space/CreateSpaceParams.kt:23-35)
struct CreateSpaceParams {
    std::string name;
    std::string topic;
    std::string roomAliasName;
    bool isPublic = false;
    bool isFederated = true;

    // Space defaults:
    std::string roomType = "m.space";    // RoomType.SPACE
    int eventsDefault = 100;             // PowerLevelsContent.events_default

    std::vector<std::string> inviteList;
    std::string powerLevelContentOverrideJson;
};

// SpaceHierarchyData
struct SpaceHierarchyData {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string canonicalAlias;
    int numJoinedMembers = 0;
    std::string joinRule;
    bool worldReadable = false;
    bool guestCanJoin = false;
    std::string roomType;
    std::string order;
    bool suggested = false;
};

// ==== Identity / 3PID Models ====

// ThreePid (session/identity/ThreePid.kt:24-44)
enum class ThreePidMedium { EMAIL, MSISDN };

struct ThreePid {
    ThreePidMedium medium = ThreePidMedium::EMAIL;
    std::string value;  // email address or phone number

    std::string toMedium() const {
        return medium == ThreePidMedium::EMAIL ? "email" : "msisdn";
    }

    static ThreePid email(const std::string& addr) {
        return {ThreePidMedium::EMAIL, addr};
    }
    static ThreePid msisdn(const std::string& phone) {
        return {ThreePidMedium::MSISDN, phone};
    }
};

// FoundThreePid (session/identity/FoundThreePid.kt:19-22)
struct FoundThreePid {
    ThreePid threePid;
    std::string matrixId;
};

// SharedState
struct SharedState {
    std::string clientSecret;
    std::string sessionId;
    int nextLinkAttempt = 0;
};

// IdentityServiceError
struct IdentityServiceError {
    std::string error;
    std::string errorDescription;
};

// SignInvitationResult
struct SignInvitationResult {
    std::string mxId;
    std::string signDisplayName;
    std::string token;
    std::string roomId;              // optional, for room-specific invites
};

// ==== Account Data ====

struct UserAccountDataEvent {
    std::string type;                // e.g. "m.direct", "m.push_rules"
    std::string contentJson;         // raw JSON content
};

// ==== Sync Config ====
struct SyncConfig {
    int timeoutMs = 30000;
    std::string filterJson;          // raw JSON filter body
    std::string since;               // sync token
    bool fullState = false;
    bool setPresence = true;
    std::string presenceStatus;      // "online", "offline", "unavailable"
};

// ==== Content Attachment ====
struct ContentAttachmentData {
    enum class Type { FILE, IMAGE, VIDEO, AUDIO, VOICE, LOCATION, CONTACT, UNKNOWN };

    Type type = Type::UNKNOWN;
    std::string mimeType;
    std::string name;
    std::string path;                // local file path
    std::string url;                 // remote URL
    int64_t size = 0;
    int width = 0;
    int height = 0;
    int durationMs = 0;             // for audio/video
    double latitude = 0;
    double longitude = 0;

    bool isImage() const { return type == Type::IMAGE; }
    bool isVideo() const { return type == Type::VIDEO; }
    bool isAudio() const { return type == Type::AUDIO || type == Type::VOICE; }
    bool isFile() const { return type == Type::FILE; }
};

// ==== MatrixUrls ====
struct MatrixUrls {
    std::string baseUrl;
    std::string identityServerUrl;
    bool hasIdentityServer() const { return !identityServerUrl.empty(); }
};

// ==== FederationVersion ====
enum class FederationVersion {
    UNKNOWN, V1, V2
};
inline FederationVersion federationVersionFromString(const std::string& s) {
    if (s == "v1") return FederationVersion::V1;
    if (s == "v2") return FederationVersion::V2;
    return FederationVersion::UNKNOWN;
}

// ==== CacheStrategy ====
enum class CacheStrategy {
    DEFAULT, NO_CACHE, SHORT_LIVED, LONG_LIVED
};

} // namespace session_models
} // namespace pcp
