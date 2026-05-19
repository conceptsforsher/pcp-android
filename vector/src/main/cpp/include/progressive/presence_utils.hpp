#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ---- User Presence ----

enum class Presence { Online, Offline, Unavailable, Unknown };

struct PresenceInfo {
    std::string userId;
    Presence presence = Presence::Offline;
    int64_t lastActiveAgoMs = 0;     // ms since last activity
    std::string statusMessage;       // custom status text
    bool isFromCache = false;         // may be stale
};

// Parse presence from Matrix API response.
PresenceInfo parsePresence(const std::string& userId, const std::string& apiResponseJson);

// Format presence as human-readable text.
std::string formatPresence(Presence presence);

// Format presence with timestamp: "Online", "Away 5m", "Offline 2h"
std::string formatPresenceWithTime(Presence presence, int64_t lastActiveAgoMs);

// Get an emoji indicator for presence: 🟢🟡🔴⚫
std::string getPresenceIndicator(Presence presence);

// Check if presence data is stale (>5 minutes old).
bool isPresenceStale(int64_t lastUpdatedMs);

// Format a custom status message for display (truncate if needed).
std::string formatStatusMessage(const std::string& message, int maxLen = 80);

// Combine presence + status into one display line.
std::string formatPresenceLine(const PresenceInfo& info);

// ---- Activity Timer ----

struct ActivityTimer {
    int64_t startedAtMs = 0;
    int64_t pausedAtMs = 0;
    int64_t totalPausedMs = 0;
    bool isRunning = false;
    bool isPaused = false;
};

class UserActivityTimer {
public:
    void start();
    void pause();
    void resume();
    void stop();

    // Get elapsed time in ms (excluding paused time).
    int64_t elapsedMs() const;

    // Get formatted elapsed time: "2h 15m 30s"
    std::string elapsedFormatted() const;

    // Check if timer is active.
    bool running() const { return isRunning_; }

private:
    int64_t startMs_ = 0;
    int64_t pauseStart_ = 0;
    int64_t totalPaused_ = 0;
    bool isRunning_ = false;
    bool isPaused_ = false;
    int64_t now() const;
};

// ---- Matrix Presence API models ----

// Original Kotlin: PresenceEnum.kt
enum class UserPresence {
    ONLINE,
    OFFLINE,
    UNAVAILABLE,
    UNKNOWN
};

// Convert UserPresence to Matrix API string representation
inline const char* userPresenceToString(UserPresence p) {
    switch (p) {
        case UserPresence::ONLINE:      return "online";
        case UserPresence::OFFLINE:     return "offline";
        case UserPresence::UNAVAILABLE: return "unavailable";
        default:                        return "offline";
    }
}

// Parse a Matrix API presence string to UserPresence
inline UserPresence userPresenceFromString(const std::string& s) {
    if (s == "online")      return UserPresence::ONLINE;
    if (s == "unavailable") return UserPresence::UNAVAILABLE;
    if (s == "offline")     return UserPresence::OFFLINE;
    return UserPresence::UNKNOWN;
}

// Original Kotlin: UserPresence.kt (data class)
struct PresenceUserInfo {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    UserPresence presence = UserPresence::OFFLINE;
    int64_t lastActiveAgoMs = 0;
    std::string statusMsg;
    bool currentlyActive = false;
};

// ---- Presence JSON builders ----

// Original Kotlin: PUT /presence/{userId}/status
// Builds the JSON body: {"presence":"online","status_msg":"..."}
std::string buildPresenceEvent(UserPresence presence, const std::string& statusMsg = "");

// Original Kotlin: POST /presence/list/{userId}
// Builds the JSON body: {"invite":[...],"drop":[...]}
std::string buildPresenceList(const std::vector<std::string>& inviteUserIds,
                               const std::vector<std::string>& dropUserIds);

// ---- Presence JSON parsers ----

// Original Kotlin: parses PUT/POST presence response (or presence event from sync)
// Returns the UserPresence value from the JSON
PresenceUserInfo parsePresenceEvent(const std::string& userId, const std::string& json);

// Original Kotlin: parses POST /presence/list response
std::vector<PresenceUserInfo> parsePresenceList(const std::string& json);

// Original Kotlin: format "last seen" / "last active" human-readable string
// e.g. "active now", "5m ago", "2h ago", "3d ago", "Jan 5"
std::string formatLastActiveTime(int64_t lastActiveAgoMs, bool currentlyActive);

// ---- PresenceSyncResult (from sync) ----

// Original Kotlin: PresenceSyncHandler.kt — processed presence events from /sync
struct PresenceSyncResult {
    std::vector<PresenceUserInfo> usersChanged;  // users whose presence changed
    int totalUsers = 0;                          // total presence events in this sync batch
};

// Process presence events from a /sync presence block.
// Iterates over presence events, parses each, and returns the aggregated result.
PresenceSyncResult processPresenceSync(const std::vector<Event>& events);

// Merge a PresenceUserInfo from sync into a local presence map (insert or update).
// Returns true if the presence actually changed (user was new or state differed).
bool updatePresenceInfo(std::unordered_map<std::string, PresenceUserInfo>& localState,
                        const PresenceUserInfo& incoming);

// ---- Presence List Request / Response ----

// Original Kotlin: POST /presence/list/{userId}
struct PresenceListRequest {
    std::vector<std::string> userIds;  // user IDs to query presence for
};

// Build JSON body for POST /presence/list/{userId} — list of user IDs to query.
std::string buildPresenceListRequest(const std::vector<std::string>& userIds);

// Original Kotlin: response from POST /presence/list/{userId}
struct PresenceListResponse {
    std::unordered_map<std::string, PresenceUserInfo> presenceList;  // userId -> presence info
};

// Parse the response from POST /presence/list/{userId}.
PresenceListResponse parsePresenceListResponse(const std::string& json);

// Original Kotlin: PUT /presence/{userId}/status
// Build JSON body for updating own presence status.
std::string buildSetPresenceRequest(UserPresence presence, const std::string& statusMsg = "");

// ---- Presence Update Info (for cache) ----

// Original Kotlin: combined presence update with timestamp
struct PresenceUpdateInfo {
    std::string userId;
    UserPresence presence = UserPresence::OFFLINE;
    std::string statusMsg;
    bool currentlyActive = false;
    int64_t lastActiveAgoMs = 0;
    int64_t ts = 0;  // epoch ms of this update
};

// Format presence timestamp in human-readable form:
// "Active now", "Active 5m ago", "Active 2h ago", "Inactive"
std::string formatPresenceTimestamp(const PresenceUpdateInfo& info);

// Check if a user is currently active (active within the last 5 minutes).
bool isCurrentlyActive(const PresenceUpdateInfo& info);

} // namespace progressive
