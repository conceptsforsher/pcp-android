#ifndef PROGRESSIVE_DESYNC_DETECTOR_HPP
#define PROGRESSIVE_DESYNC_DETECTOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

namespace progressive {

struct ServerEvent {
    std::string eventId;
    std::string serverName;      // matrix.org, server.com
    int64_t timestamp = 0;
    bool present = true;         // confirmed seen on this server
};

struct DesyncReport {
    bool hasDesync = false;
    std::string roomId;
    int totalEventsCompared = 0;
    int missingOnCurrent = 0;    // events on other servers but not here
    int missingOnOther = 0;      // events here but not on other servers
    std::vector<std::string> missingEventIds;
    std::string missingServer;   // which server is missing events
    int64_t lastCheckMs = 0;
};

class DesyncDetector {
public:
    // Register an event as seen on a specific server.
    void trackEvent(const std::string& eventId, const std::string& serverName, int64_t timestamp);

    // Run a desync check for a room across known servers.
    // Returns a report if desync detected, or empty report if all good.
    DesyncReport checkDesync(const std::string& roomId, const std::string& currentServer);

    // Get all known servers for a room.
    std::vector<std::string> getServers(const std::string& roomId) const;

    // Remove events for a room (when leaving).
    void removeRoom(const std::string& roomId);

    // Check if a specific event exists on a server.
    bool hasEventOnServer(const std::string& eventId, const std::string& serverName) const;

    // Compute a short summary of the desync: "3 events missing on matrix.org"
    static std::string formatDesyncWarning(const DesyncReport& report);

    // Export desync report as JSON.
    static std::string reportToJson(const DesyncReport& report);

    // Check if we should run a check (interval passed).
    bool shouldCheck(int64_t lastCheckMs, int intervalMinutes) const;

    void clear();
    size_t eventCount() const { return eventTimestamps_.size(); }

private:
    // key: serverName → set of eventIds
    std::unordered_map<std::string, std::unordered_set<std::string>> serverEvents_;
    // key: eventId → timestamp
    std::unordered_map<std::string, int64_t> eventTimestamps_;
};

// ---- Desync Check Result ----

// Original Kotlin: result of a multi-factor desync check
struct DesyncCheckResult {
    bool isDesynchronized = false;
    std::string reason;                  // human-readable explanation
    std::string suggestedAction;         // "re-sync", "clear_cache", "re-login"
    int64_t lastSyncAgeMs = 0;           // ms since last successful sync
    int missingEvents = 0;               // events found missing in local timeline
    int extraEvents = 0;                 // events found locally but not on server
};

// Run a multi-factor desync check:
//   - Stream ordering gaps
//   - Missing events in timeline
//   - Read marker conflicts
//   - Membership inconsistencies
DesyncCheckResult checkSyncDesynchronization(
    const std::string& roomId,
    int64_t lastSyncMs,
    const std::vector<std::string>& localEventIds,
    const std::vector<std::string>& serverEventIds,
    const std::string& localReadMarker,
    const std::string& serverReadMarker,
    const std::string& localMembership,
    const std::string& serverMembership
);

// ---- Desync Cause Classification ----

// Original Kotlin: heuristic classification of desync root cause
enum class DesyncCause {
    NETWORK_GAP,
    CACHE_CORRUPTION,
    SERVER_BUG,
    RACE_CONDITION,
    UNKNOWN
};

// Diagnose the root cause of a desync from the check result.
DesyncCause diagnoseDesyncCause(const DesyncCheckResult& result);

// Get the recommended recovery action string based on the diagnosed cause.
// Returns: "re-sync", "clear_cache", "re-login", or "none"
std::string getDesyncRecoveryAction(DesyncCause cause);

// Get the recovery action from a DesyncCheckResult directly.
std::string getDesyncRecoveryAction(const DesyncCheckResult& result);

} // namespace progressive

#endif // PROGRESSIVE_DESYNC_DETECTOR_HPP
