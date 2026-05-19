#ifndef PROGRESSIVE_SESSION_MANAGER_HPP
#define PROGRESSIVE_SESSION_MANAGER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

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

#endif // PROGRESSIVE_SESSION_MANAGER_HPP
