#ifndef PROGRESSIVE_CONNECTION_MONITOR_HPP
#define PROGRESSIVE_CONNECTION_MONITOR_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct ConnectionState {
    bool isConnected = true;
    int64_t lastConnectedMs = 0;     // last time we were connected
    int64_t disconnectedAtMs = 0;    // when we lost connection
    int64_t downtimeMs = 0;          // how long we've been disconnected
    int reconnectAttempts = 0;
    int64_t lastReconnectAttemptMs = 0;

    // Computed
    bool wasEverConnected = false;
    std::string downtimeText;        // "2 minutes ago"
    std::string statusText;          // "Connected" or "Connection lost 2 minutes ago"
};

class ConnectionMonitor {
public:
    // Call when connection is established.
    void onConnected();

    // Call when connection is lost.
    void onDisconnected();

    // Call when a reconnection attempt is made.
    void onReconnectAttempt();

    // Get current state with computed downtime text.
    ConnectionState getState() const;

    // Check if we've been disconnected too long (threshold in seconds).
    bool isDisconnectedTooLong(int thresholdSeconds = 300) const;

    // Format downtime as human-readable text.
    static std::string formatDowntime(int64_t downtimeMs);

    // Format full status text for the banner.
    static std::string formatStatusText(const ConnectionState& state);

    // Get the recommended banner color based on downtime duration.
    static std::string getBannerColor(int64_t downtimeMs);

    void reset();

private:
    ConnectionState state_;
    int64_t nowMs() const;
};

} // namespace progressive

#endif // PROGRESSIVE_CONNECTION_MONITOR_HPP
