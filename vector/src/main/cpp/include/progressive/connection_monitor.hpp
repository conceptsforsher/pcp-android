#pragma once

#include <string>
#include <vector>
#include <deque>
#include <cstdint>

namespace progressive {

// ================================================================
// Connection State Machine
// ================================================================

// Detailed connection state (extended from simple connected/disconnected).
// Original Kotlin: connection state tracking in Matrix SDK
enum class ConnectionDetailState {
    DISCONNECTED = 0,
    CONNECTING_DNS = 1,
    CONNECTING_TCP = 2,
    CONNECTING_TLS = 3,
    CONNECTED = 4,
    RECONNECTING = 5,
    CLOSING = 6,
    CLOSED = 7
};

enum class ConnectionFailReason {
    DNS_FAILURE = 0,
    CONNECT_TIMEOUT = 1,
    TLS_HANDSHAKE_FAILURE = 2,
    CONNECTION_REFUSED = 3,
    NETWORK_UNREACHABLE = 4,
    RESET_BY_PEER = 5,
    READ_TIMEOUT = 6,
    WRITE_TIMEOUT = 7,
    SSL_EXPIRED = 8,
    SSL_UNTRUSTED = 9,
    UNKNOWN = 10
};

enum class ConnectionHealth {
    HEALTHY = 0,
    DEGRADED = 1,
    UNSTABLE = 2,
    DEAD = 3
};

const char* connectionDetailStateToString(ConnectionDetailState state);
const char* connectionFailReasonToString(ConnectionFailReason reason);
const char* connectionHealthToString(ConnectionHealth health);

struct ConnectionInfo {
    ConnectionDetailState state = ConnectionDetailState::DISCONNECTED;
    std::string host;
    int port = 443;
    bool isSecure = true;
    std::string protocol;          // "https", "wss", "matrix"
    int64_t lastConnectTime = 0;
    int64_t lastDisconnectTime = 0;
    int failCount = 0;
    int consecutiveFailures = 0;
};

struct ConnectionConfig {
    int connectTimeoutMs = 10000;
    int readTimeoutMs = 30000;
    int writeTimeoutMs = 10000;
    int pingIntervalMs = 30000;
    int maxRetries = 3;
    int retryDelayMs = 2000;
    int keepAliveMs = 300000;      // 5 minutes
};

// ================================================================
// Connection Health Check
// ================================================================

ConnectionHealth checkConnectionHealth(
    const ConnectionInfo& info,
    double currentLatencyMs = 0.0);

// ================================================================
// Connection Pool
// ================================================================

struct ConnectionPoolConfig {
    int maxConnections = 16;
    int maxIdleConnections = 8;
    int keepAliveMs = 300000;            // 5 minutes
    int maxRequestsPerConnection = 100;
};

struct ConnectionPoolEntry {
    std::string id;                      // "host:port:protocol"
    ConnectionInfo info;
    bool inUse = false;
    int64_t createdAt = 0;
    int64_t lastUsedAt = 0;
    int requestCount = 0;
};

struct ConnectionPoolStats {
    int active = 0;                      // currently in use
    int idle = 0;                        // available for reuse
    int total = 0;                       // total in pool
};

class ConnectionPool {
public:
    explicit ConnectionPool(const ConnectionPoolConfig& config = ConnectionPoolConfig{});

    // Get a connection from the pool (or create new). Returns entry id.
    std::string getConnection(const std::string& host, int port, bool isSecure);

    // Release a connection back to pool for reuse.
    void releaseConnection(const std::string& id, bool keepAlive = true);

    // Close idle connections older than threshold.
    void closeIdleConnections(int64_t idleThresholdMs = 300000);

    // Get current pool stats.
    ConnectionPoolStats getPoolStats() const;

    // Clear all connections.
    void clear();

    size_t size() const { return pool_.size(); }

private:
    ConnectionPoolConfig config_;
    std::deque<ConnectionPoolEntry> pool_;
    int seq_ = 0;
    std::string nextId();
};

// ================================================================
// Connection Lifecycle Callbacks
// ================================================================

// Original Kotlin: connection lifecycle events in Matrix SDK
struct ConnectionLifecycle {
    // Called when connecting begins (DNS phase).
    void onConnecting(const std::string& host, int port);

    // Called when fully connected.
    void onConnected(const std::string& host, int port, const std::string& protocol);

    // Called when disconnected.
    void onDisconnected(const std::string& host, ConnectionFailReason reason);

    // Called on connection error.
    void onError(const std::string& host, ConnectionFailReason reason,
        const std::string& message);

    // Get latest connection info.
    const ConnectionInfo& currentInfo() const { return info_; }

    // Get fail count.
    int getFailCount() const { return info_.failCount; }

private:
    ConnectionInfo info_;
};

// ================================================================
// Error Classification
// ================================================================

// Map a native/errno error to a ConnectionFailReason.
ConnectionFailReason classifyConnectionError(int nativeErrorCode,
    const std::string& errorMessage = "");

// ================================================================
// Existing: Connection State / Monitor
// ================================================================

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
