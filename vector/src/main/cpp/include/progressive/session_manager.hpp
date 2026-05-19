#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Login Type ----
// Original Kotlin: org.matrix.android.sdk.api.auth.LoginType

enum class LoginType {
    PASSWORD,
    SSO,
    UNSUPPORTED,
    CUSTOM,
    DIRECT,
    UNKNOWN,
    QR
};

std::string loginTypeToString(LoginType type);
LoginType loginTypeFromString(const std::string& name);

// ---- Session Credentials ----
// Original Kotlin: org.matrix.android.sdk.api.auth.data.Credentials

struct SessionCredentials {
    std::string userId;           // @Json(name = "user_id")
    std::string accessToken;      // @Json(name = "access_token")
    std::string deviceId;         // @Json(name = "device_id")
    std::string homeServer;       // @Json(name = "home_server")
    std::string refreshToken;     // @Json(name = "refresh_token"), optional

    bool isValid() const {
        return !userId.empty() && !accessToken.empty();
    }

    bool hasRefreshToken() const {
        return !refreshToken.empty();
    }
};

// Serialize session credentials for persistent storage.
std::string buildSessionCredentials(const SessionCredentials& creds);

// Deserialize session credentials from storage.
SessionCredentials parseSessionCredentials(const std::string& json);

// ---- Session Params ----
// Original Kotlin: org.matrix.android.sdk.api.auth.data.SessionParams

// Original Kotlin: org.matrix.android.sdk.api.auth.data.SessionParams
// Expanded with sessionId, refreshToken, and homeserverCapabilities.
struct SessionParams {
    std::string userId;                       // shortcut from credentials.userId
    std::string accessToken;                  // shortcut from credentials.accessToken
    std::string deviceId;                     // shortcut from credentials.deviceId
    std::string homeServerUrl;                // homeServerConnectionConfig.homeServerUri
    std::string homeServerUrlBase;            // homeServerConnectionConfig.homeServerUriBase (after redirect)
    std::string identityServerUrl;            // homeServerConnectionConfig.identityServerUri
    std::string homeServerCapabilities;       // JSON string of HomeServerCapabilities
    std::string sessionId;                    // stable session identifier (derived from userId+deviceId)
    std::string refreshToken;                 // OAuth refresh token (optional)
    bool isTokenValid = true;
    bool isUserRegistered = false;
    LoginType loginType = LoginType::UNKNOWN;
    int64_t creationTimestampMs = 0;          // when session was created

    bool isValid() const {
        return !userId.empty() && !accessToken.empty() && !homeServerUrl.empty();
    }

    bool hasIdentityServer() const {
        return !identityServerUrl.empty();
    }
};

// Serialize session params to JSON.
std::string buildSessionParams(const SessionParams& params);

// Parse session params from JSON.
SessionParams parseSessionParams(const std::string& json);

// ---- Session State ----
// Original Kotlin: org.matrix.android.sdk.api.session.SessionLifecycleObserver
//                     org.matrix.android.sdk.internal.session.SessionState

enum class SessionState {
    UNINITIALIZED,
    INITIALIZING,
    INITIALIZED,
    OPENED,
    STARTED,
    STOPPED,
    DESTROYED
};

std::string sessionStateToString(SessionState state);
SessionState sessionStateFromString(const std::string& name);

// ---- Session Lifecycle Tracker ----
// Original Kotlin: SessionLifecycleObserver lifecycle transitions
// Tracks session lifecycle state transitions with validation.

struct SessionLifecycleState {
    SessionState currentState = SessionState::UNINITIALIZED;
    int64_t lastTransitionMs = 0;
    int64_t initializationTimeMs = 0;
    int64_t startedTimeMs = 0;

    // Check if a transition to the target state is allowed.
    bool canTransitionTo(SessionState target) const;

    // Attempt the transition; returns true on success, false if disallowed.
    bool transitionTo(SessionState target);

    bool isOpen() const {
        return currentState == SessionState::STARTED;
    }

    bool isActive() const {
        return currentState == SessionState::STARTED
            || currentState == SessionState::OPENED;
    }

    bool isStopped() const {
        return currentState == SessionState::STOPPED
            || currentState == SessionState::DESTROYED;
    }

    // How long has the session been in the current state (ms).
    int64_t elapsedInCurrentStateMs(int64_t nowMs) const {
        return nowMs - lastTransitionMs;
    }
};

// ---- Multi-Session Management ----

struct SessionInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string homeServer;
    std::string deviceId;
    int unreadCount = 0;
    int highlightCount = 0;
    bool isActive = false;         // currently in use
    bool isLoggedIn = true;
    int64_t lastSyncMs = 0;
    int sessionIndex = 0;          // 1, 2, 3... for user-facing numbering
};

struct SessionList {
    std::vector<SessionInfo> sessions;
    std::string activeUserId;
    int totalUnread = 0;
    int totalHighlights = 0;
};

// Compute session list with unread counts and ordering.
SessionList computeSessionList(const std::vector<SessionInfo>& sessions,
    const std::string& activeUserId);

// Assign session indices (1, 2, 3...) based on login order.
void assignSessionIndices(std::vector<SessionInfo>& sessions);

// Sort sessions: active first, then by last sync time.
void sortSessions(std::vector<SessionInfo>& sessions);

// Format session badge text for drawer/account switcher.
std::string formatSessionBadge(const SessionInfo& session);

// Format session info for display in account switcher.
std::string formatSessionInfo(const SessionInfo& session);

// Check if there are any pending notifications across all sessions.
bool hasPendingNotifications(const SessionList& list);

// Get the best session to switch to (most recent activity).
std::string getRecommendedSession(const SessionList& list, const std::string& excludeUserId);

// ---- Session Persistence ----

struct SessionPersistence {
    std::string userId;
    std::string accessToken;
    std::string refreshToken;
    std::string homeServerUrl;
    std::string deviceId;
    int64_t lastUsedMs = 0;
    bool isActive = false;
};

// Serialize session data for persistent storage.
std::string serializeSession(const SessionPersistence& session);

// Deserialize session data from storage.
SessionPersistence deserializeSession(const std::string& data);

// Check if a session needs refresh (token expired or close to expiry).
bool needsTokenRefresh(const SessionPersistence& session, int64_t tokenExpiryMs);

// Format session list as JSON.
std::string sessionListToJson(const SessionList& list);

} // namespace progressive
