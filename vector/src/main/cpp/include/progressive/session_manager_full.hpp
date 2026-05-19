#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Session Manager — multi-account session management
//
// Faithful port from Element Android original sources:
//   Session.kt — session interface (open/close, services, myUserId, sessionId)
//   SavedSessionParams.kt — session configuration (credentials, home server,
//     isTokenValid, loginType, shortcuts for userId/deviceId/homeServerUrl)
//   Credentials.kt — auth data (userId, accessToken, refreshToken,
//     homeServer, deviceId, discoveryInformation, sessionId via md5)
//   ActiveSessionHolder.kt — singleton holder (setActiveSession,
//     clearActiveSession, hasActiveSession, getSafeActiveSession)
//   LoginType.kt — authentication method enum
//
// Covers:
//   1. Session state management (active/inactive/closed)
//   2. Multi-session support (up to 5 simultaneous sessions)
//   3. Session credentials storage
//   4. Session ID computation
//   5. Home server configuration
//   6. Token validation and refresh
//   7. Session lifecycle (open/close/cleanup)
// ================================================================

// ---- Login Type ----
// Original: LoginType enum (PASSWORD, SSO, TOKEN, etc.)

enum class SessionLoginType {
    UNKNOWN = 0,
    PASSWORD = 1,
    SSO = 2,
    TOKEN = 3,
    OIDC = 4,
};

const char* sessionLoginTypeToString(SessionLoginType type);
SessionLoginType sessionLoginTypeFromString(const std::string& s);

// ---- Discovery Information ----
// Original: SessionDiscoveryInfo — from well-known response

struct SessionDiscoveryInfo {
    std::string baseUrl;
    std::string identityServer;
    bool valid = false;
};

// ---- Credentials ----
// Original: Credentials.kt (userId, accessToken, refreshToken, homeServer,
//   deviceId, discoveryInformation)
// Original: sessionId() = md5("$userId|$deviceId")

struct SessionCredentials {
    std::string userId;              // @alice:example.org (required)
    std::string accessToken;         // syt_... (required)
    std::string refreshToken;        // Optional refresh token
    std::string homeServer;          // Deprecated — extract from userId
    std::string deviceId;            // ABCDEFGHIJ
    SessionDiscoveryInfo discoveryInfo;
    bool valid = false;

    // Original: sessionId() = md5("$userId|$deviceId")
    // We compute a simple hash-based session ID
    std::string computeSessionId() const;

    // Check if the token is still present
    bool hasToken() const { return !accessToken.empty(); }

    // Extract home server from userId (@user:example.org → example.org)
    std::string extractHomeServer() const;
};

// ---- Home Server Connection Config ----
// Original: HomeServerConnectionConfig (homeServerUri, homeServerUriBase, identityServerUri)

struct HomeServerConfig {
    std::string homeServerUrl;       // User-entered URL
    std::string homeServerUrlBase;   // Actual URL (after redirect)
    std::string homeServerHost;      // Host portion
    std::string identityServerUrl;   // Optional identity server
    bool valid = false;
};

// ---- Session Params ----
// Original: SavedSessionParams.kt (credentials, homeServerConnectionConfig,
//   isTokenValid, loginType)
// Original: Shortcuts — userId, deviceId, homeServerUrl, homeServerUrlBase,
//   homeServerHost, defaultIdentityServerUrl

struct SavedSessionParams {
    SessionCredentials credentials;
    HomeServerConfig homeServerConfig;
    bool isTokenValid = false;
    SessionLoginType loginType = SessionLoginType::UNKNOWN;

    // Shortcuts (from original)
    std::string userId() const { return credentials.userId; }
    std::string deviceId() const { return credentials.deviceId; }
    std::string homeServerUrl() const { return homeServerConfig.homeServerUrl; }
    std::string homeServerUrlBase() const { return homeServerConfig.homeServerUrlBase; }
    std::string homeServerHost() const { return homeServerConfig.homeServerHost; }
    std::string defaultIdentityServerUrl() const { return homeServerConfig.identityServerUrl; }
    std::string sessionId() const { return credentials.computeSessionId(); }
    bool valid() const { return credentials.valid && isTokenValid; }
};

// ---- Session State ----
// Original: Session state transitions

enum class SessionLifecycleState {
    CREATED = 0,         // Constructed but not opened
    OPENING = 1,         // Opening (sync starting)
    OPENED = 2,          // Fully open and syncing
    CLOSING = 3,         // Closing
    CLOSED = 4,          // Closed
    ERROR = 5,           // Error state (token invalid, etc.)
};

const char* sessionStateToString(SessionLifecycleState state);

// ---- Session Info ----
// Original: Session interface properties (myUserId, sessionId, isOpenable)

struct SavedSessionInfo {
    std::string sessionId;           // Original: sessionId
    std::string userId;              // Original: myUserId = sessionParams.userId
    std::string deviceId;
    std::string displayName;         // From account data
    std::string avatarUrl;           // From account data
    std::string homeServerUrl;
    std::string homeServerUrlBase;
    std::string identityServerUrl;
    SessionLoginType loginType = SessionLoginType::UNKNOWN;
    SessionLifecycleState state = SessionLifecycleState::CREATED;
    bool isOpenable = false;         // Original: isOpenable — has valid token
    bool isActive = false;           // Current active session
    bool isSyncing = false;          // Sync service running
    int64_t createdAtMs = 0;
    int64_t lastSyncMs = 0;
    int64_t tokenExpiresAtMs = 0;   // 0 = no expiry
    std::string pushRuleGlobal;     // Global push rule (all messages, mentions, none)
};

// ---- Session Manager ----
// Original: ActiveSessionHolder.kt (singleton with AtomicReference<Session?>)

class SessionManager {
public:
    SessionManager();

    // ====== Session Lifecycle ======
    // Original: ActiveSessionHolder.setActiveSession(session)

    // Create a new session from credentials.
    // Returns the session ID.
    std::string createSession(const SessionCredentials& creds, const HomeServerConfig& config,
                               SessionLoginType loginType, std::string& error);

    // Open a session (start sync, activate services).
    // Original: Session.open()
    bool openSession(const std::string& sessionId, std::string& error);

    // Close a session (stop sync, cleanup).
    // Original: Session.close()
    bool closeSession(const std::string& sessionId);

    // Remove a session entirely (sign out).
    bool removeSession(const std::string& sessionId);

    // Clear all sessions.
    void clearAllSessions();

    // ====== Active Session ======
    // Original: ActiveSessionHolder.setActiveSession / clearActiveSession / hasActiveSession

    // Set the active session (only one at a time).
    bool setActiveSession(const std::string& sessionId);

    // Clear the active session.
    void clearActiveSession();

    // Check if there is an active session.
    bool hasActiveSession() const;

    // Get the active session info.
    bool getActiveSession(SavedSessionInfo& out) const;

    // Get a session by ID.
    bool getSession(const std::string& sessionId, SavedSessionInfo& out) const;

    // Get a session by user ID.
    bool getSessionByUser(const std::string& userId, SavedSessionInfo& out) const;

    // ====== Session Queries ======
    // Original: Session properties

    // Get all sessions (sorted by last activity).
    std::vector<SavedSessionInfo> getAllSessions() const;

    // Get session count.
    int sessionCount() const { return static_cast<int>(sessions_.size()); }

    // Check if a session exists.
    bool hasSession(const std::string& sessionId) const;

    // Check if a user already has a session.
    bool hasUserSession(const std::string& userId) const;

    // ====== Token Management ======

    // Update access token for a session.
    bool updateAccessToken(const std::string& sessionId, const std::string& newToken);

    // Update refresh token.
    bool updateRefreshToken(const std::string& sessionId, const std::string& newRefreshToken);

    // Mark token as invalid.
    void invalidateToken(const std::string& sessionId);

    // Check if token is expired.
    bool isTokenExpired(const std::string& sessionId) const;

    // ====== Session Metadata ======

    // Set display name for a session.
    void setDisplayName(const std::string& sessionId, const std::string& name);

    // Set avatar URL for a session.
    void setAvatarUrl(const std::string& sessionId, const std::string& url);

    // Update sync timestamp.
    void updateSyncTimestamp(const std::string& sessionId);

    // ====== Push Rules ======

    // Set global push rule for a session.
    void setPushRule(const std::string& sessionId, const std::string& rule);

    // ====== Serialization ======

    // Export session info as JSON.
    std::string sessionToJson(const SavedSessionInfo& session) const;

    // Export all sessions as JSON array.
    std::string allSessionsToJson() const;

    // Export session params as JSON.
    std::string paramsToJson(const SavedSessionParams& params) const;

    // ====== Session ID ======

    // Compute session ID from userId + deviceId.
    // Original: md5("$userId|$deviceId")
    static std::string computeSessionId(const std::string& userId, const std::string& deviceId);

private:
    std::unordered_map<std::string, SavedSessionInfo> sessions_;
    std::string activeSessionId_;

    SavedSessionInfo* findSession(const std::string& sessionId);
    const SavedSessionInfo* findSession(const std::string& sessionId) const;

    // Simple hash for session ID (replaces MD5 from original).
    static std::string simpleHash(const std::string& input);
};

} // namespace progressive
