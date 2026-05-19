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

} // namespace progressive

#endif // PROGRESSIVE_DESYNC_DETECTOR_HPP
