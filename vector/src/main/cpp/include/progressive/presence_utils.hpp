#ifndef PROGRESSIVE_PRESENCE_UTILS_HPP
#define PROGRESSIVE_PRESENCE_UTILS_HPP

#include <string>
#include <cstdint>

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

} // namespace progressive

#endif // PROGRESSIVE_PRESENCE_UTILS_HPP
