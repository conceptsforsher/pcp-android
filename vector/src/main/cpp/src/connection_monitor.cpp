#include "progressive/connection_monitor.hpp"
#include <sstream>
#include <chrono>
#include <cmath>

namespace progressive {

int64_t ConnectionMonitor::nowMs() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void ConnectionMonitor::onConnected() {
    state_.isConnected = true;
    state_.lastConnectedMs = nowMs();
    state_.wasEverConnected = true;
    state_.downtimeMs = 0;
    state_.reconnectAttempts = 0;
}

void ConnectionMonitor::onDisconnected() {
    if (state_.isConnected) {
        state_.isConnected = false;
        state_.disconnectedAtMs = nowMs();
    }
    state_.downtimeMs = nowMs() - state_.disconnectedAtMs;
}

void ConnectionMonitor::onReconnectAttempt() {
    state_.reconnectAttempts++;
    state_.lastReconnectAttemptMs = nowMs();
}

ConnectionState ConnectionMonitor::getState() const {
    ConnectionState copy = state_;
    if (!copy.isConnected && copy.disconnectedAtMs > 0) {
        copy.downtimeMs = nowMs() - copy.disconnectedAtMs;
    }
    copy.downtimeText = formatDowntime(copy.downtimeMs);
    copy.statusText = formatStatusText(copy);
    return copy;
}

bool ConnectionMonitor::isDisconnectedTooLong(int thresholdSeconds) const {
    if (state_.isConnected) return false;
    return getState().downtimeMs > thresholdSeconds * 1000LL;
}

std::string ConnectionMonitor::formatDowntime(int64_t downtimeMs) {
    if (downtimeMs <= 0) return "just now";

    int64_t seconds = downtimeMs / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;

    // Under 10 seconds: "just now"
    if (seconds < 10) return "just now";

    // Under 60 seconds: "X seconds ago"
    if (seconds < 60) return std::to_string(seconds) + " seconds ago";

    // Under 60 minutes: "X minutes ago"
    if (minutes == 1) return "1 minute ago";
    if (minutes < 60) return std::to_string(minutes) + " minutes ago";

    // Under 24 hours: "X hours Y minutes ago"
    if (hours == 1) return "1 hour ago";
    if (hours < 24) {
        int remainingMin = minutes % 60;
        if (remainingMin == 0) return std::to_string(hours) + " hours ago";
        return std::to_string(hours) + " hours " + std::to_string(remainingMin) + " minutes ago";
    }

    // Days
    if (days == 1) return "1 day ago";
    return std::to_string(days) + " days ago";
}

std::string ConnectionMonitor::formatStatusText(const ConnectionState& state) {
    if (state.isConnected) return "Connected";

    std::ostringstream out;
    out << "Connection lost";
    if (!state.downtimeText.empty() && state.downtimeText != "just now") {
        out << " " << state.downtimeText;
    }
    if (state.reconnectAttempts > 0) {
        out << " (" << state.reconnectAttempts << " attempt"
            << (state.reconnectAttempts == 1 ? "" : "s") << ")";
    }
    return out.str();
}

std::string ConnectionMonitor::getBannerColor(int64_t downtimeMs) {
    if (downtimeMs < 30000) return "#FF9800";     // orange (0-30s)
    if (downtimeMs < 120000) return "#F44336";    // red (30s-2min)
    return "#B71C1C";                                // dark red (>2min)
}

void ConnectionMonitor::reset() {
    state_ = ConnectionState{};
}

// ================================================================
// String Conversions
// ================================================================

const char* connectionDetailStateToString(ConnectionDetailState state) {
    switch (state) {
        case ConnectionDetailState::DISCONNECTED:   return "DISCONNECTED";
        case ConnectionDetailState::CONNECTING_DNS: return "CONNECTING_DNS";
        case ConnectionDetailState::CONNECTING_TCP: return "CONNECTING_TCP";
        case ConnectionDetailState::CONNECTING_TLS: return "CONNECTING_TLS";
        case ConnectionDetailState::CONNECTED:      return "CONNECTED";
        case ConnectionDetailState::RECONNECTING:   return "RECONNECTING";
        case ConnectionDetailState::CLOSING:        return "CLOSING";
        case ConnectionDetailState::CLOSED:         return "CLOSED";
    }
    return "DISCONNECTED";
}

const char* connectionFailReasonToString(ConnectionFailReason reason) {
    switch (reason) {
        case ConnectionFailReason::DNS_FAILURE:              return "DNS_FAILURE";
        case ConnectionFailReason::CONNECT_TIMEOUT:          return "CONNECT_TIMEOUT";
        case ConnectionFailReason::TLS_HANDSHAKE_FAILURE:    return "TLS_HANDSHAKE_FAILURE";
        case ConnectionFailReason::CONNECTION_REFUSED:       return "CONNECTION_REFUSED";
        case ConnectionFailReason::NETWORK_UNREACHABLE:      return "NETWORK_UNREACHABLE";
        case ConnectionFailReason::RESET_BY_PEER:            return "RESET_BY_PEER";
        case ConnectionFailReason::READ_TIMEOUT:             return "READ_TIMEOUT";
        case ConnectionFailReason::WRITE_TIMEOUT:            return "WRITE_TIMEOUT";
        case ConnectionFailReason::SSL_EXPIRED:              return "SSL_EXPIRED";
        case ConnectionFailReason::SSL_UNTRUSTED:            return "SSL_UNTRUSTED";
        default:                                             return "UNKNOWN";
    }
}

const char* connectionHealthToString(ConnectionHealth health) {
    switch (health) {
        case ConnectionHealth::HEALTHY:   return "HEALTHY";
        case ConnectionHealth::DEGRADED:  return "DEGRADED";
        case ConnectionHealth::UNSTABLE:  return "UNSTABLE";
        case ConnectionHealth::DEAD:      return "DEAD";
    }
    return "HEALTHY";
}

// ================================================================
// Connection Health
// ================================================================
//
// Original Kotlin: connection health evaluation in Matrix SDK

ConnectionHealth checkConnectionHealth(
    const ConnectionInfo& info,
    double currentLatencyMs)
{
    if (info.state == ConnectionDetailState::DISCONNECTED ||
        info.state == ConnectionDetailState::CLOSED) {
        return ConnectionHealth::DEAD;
    }

    if (info.state == ConnectionDetailState::CONNECTING_DNS ||
        info.state == ConnectionDetailState::CONNECTING_TCP ||
        info.state == ConnectionDetailState::CONNECTING_TLS ||
        info.state == ConnectionDetailState::RECONNECTING) {
        if (info.consecutiveFailures >= 3)
            return ConnectionHealth::DEAD;
        if (info.consecutiveFailures >= 1)
            return ConnectionHealth::UNSTABLE;
        return ConnectionHealth::DEGRADED;
    }

    // CONNECTED state
    if (info.consecutiveFailures >= 5)
        return ConnectionHealth::DEAD;
    if (info.consecutiveFailures >= 3)
        return ConnectionHealth::UNSTABLE;
    if (info.failCount >= 10)
        return ConnectionHealth::DEGRADED;

    if (currentLatencyMs > 5000.0)
        return ConnectionHealth::UNSTABLE;
    if (currentLatencyMs > 1000.0)
        return ConnectionHealth::DEGRADED;

    return ConnectionHealth::HEALTHY;
}

// ================================================================
// Connection Pool
// ================================================================

ConnectionPool::ConnectionPool(const ConnectionPoolConfig& config)
    : config_(config) {}

std::string ConnectionPool::nextId() {
    return "conn_" + std::to_string(++seq_);
}

std::string ConnectionPool::getConnection(const std::string& host, int port, bool isSecure) {
    // Look for an idle connection matching host:port:secure
    for (auto& entry : pool_) {
        if (!entry.inUse &&
            entry.info.host == host &&
            entry.info.port == port &&
            entry.info.isSecure == isSecure) {
            entry.inUse = true;
            entry.lastUsedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            entry.requestCount++;
            return entry.id;
        }
    }

    // Create new connection entry
    ConnectionPoolEntry entry;
    entry.id = nextId();
    entry.info.host = host;
    entry.info.port = port;
    entry.info.isSecure = isSecure;
    entry.info.state = ConnectionDetailState::CONNECTED;
    entry.inUse = true;
    entry.createdAt = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entry.lastUsedAt = entry.createdAt;
    entry.requestCount = 0;

    pool_.push_back(entry);

    // Trim pool if exceeded max
    while (static_cast<int>(pool_.size()) > config_.maxConnections) {
        // Remove oldest idle entry
        bool removed = false;
        for (auto it = pool_.begin(); it != pool_.end(); ++it) {
            if (!it->inUse) {
                pool_.erase(it);
                removed = true;
                break;
            }
        }
        if (!removed) break; // all in use, can't trim
    }

    return entry.id;
}

void ConnectionPool::releaseConnection(const std::string& id, bool keepAlive) {
    for (auto& entry : pool_) {
        if (entry.id == id) {
            if (keepAlive &&
                entry.requestCount < config_.maxRequestsPerConnection) {
                entry.inUse = false;
                entry.lastUsedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            } else {
                entry.info.state = ConnectionDetailState::CLOSED;
            }
            return;
        }
    }
}

void ConnectionPool::closeIdleConnections(int64_t idleThresholdMs) {
    int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    auto it = pool_.begin();
    while (it != pool_.end()) {
        if (!it->inUse && (now - it->lastUsedAt) > idleThresholdMs) {
            it = pool_.erase(it);
        } else {
            ++it;
        }
    }
}

ConnectionPoolStats ConnectionPool::getPoolStats() const {
    ConnectionPoolStats stats;
    stats.total = static_cast<int>(pool_.size());
    for (const auto& e : pool_) {
        if (e.inUse) stats.active++;
        else stats.idle++;
    }
    return stats;
}

void ConnectionPool::clear() {
    pool_.clear();
}

// ================================================================
// Connection Lifecycle
// ================================================================

void ConnectionLifecycle::onConnecting(const std::string& host, int port) {
    info_.host = host;
    info_.port = port;
    info_.state = ConnectionDetailState::CONNECTING_DNS;
}

void ConnectionLifecycle::onConnected(const std::string& host, int port,
    const std::string& protocol)
{
    info_.host = host;
    info_.port = port;
    info_.protocol = protocol;
    info_.state = ConnectionDetailState::CONNECTED;
    info_.lastConnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    info_.consecutiveFailures = 0;
}

void ConnectionLifecycle::onDisconnected(const std::string& host,
    ConnectionFailReason reason)
{
    info_.host = host;
    info_.state = ConnectionDetailState::DISCONNECTED;
    info_.lastDisconnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void ConnectionLifecycle::onError(const std::string& host,
    ConnectionFailReason reason, const std::string& message)
{
    info_.host = host;
    info_.failCount++;
    info_.consecutiveFailures++;
    info_.state = ConnectionDetailState::DISCONNECTED;
    info_.lastDisconnectTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// ================================================================
// Error Classification
// ================================================================
//
// Original Kotlin: OkHttp/Retrofit error classification

ConnectionFailReason classifyConnectionError(int nativeErrorCode,
    const std::string& errorMessage)
{
    // Unix errno-based classification
    switch (nativeErrorCode) {
        case 101: // EHOSTUNREACH (varies by platform, but common)
        case 113: // EHOSTUNREACH (Linux)
            return ConnectionFailReason::NETWORK_UNREACHABLE;

        case 110: // ETIMEDOUT
            return ConnectionFailReason::CONNECT_TIMEOUT;

        case 111: // ECONNREFUSED
            return ConnectionFailReason::CONNECTION_REFUSED;

        case 104: // ECONNRESET
            return ConnectionFailReason::RESET_BY_PEER;

        default:
            break;
    }

    // Message-based classification
    if (errorMessage.empty()) return ConnectionFailReason::UNKNOWN;

    if (errorMessage.find("DNS") != std::string::npos ||
        errorMessage.find("dns") != std::string::npos ||
        errorMessage.find("hostname") != std::string::npos ||
        errorMessage.find("UnknownHost") != std::string::npos) {
        return ConnectionFailReason::DNS_FAILURE;
    }
    if (errorMessage.find("SSL") != std::string::npos ||
        errorMessage.find("TLS") != std::string::npos ||
        errorMessage.find("certificate") != std::string::npos) {
        if (errorMessage.find("expired") != std::string::npos)
            return ConnectionFailReason::SSL_EXPIRED;
        if (errorMessage.find("untrusted") != std::string::npos)
            return ConnectionFailReason::SSL_UNTRUSTED;
        return ConnectionFailReason::TLS_HANDSHAKE_FAILURE;
    }
    if (errorMessage.find("timeout") != std::string::npos ||
        errorMessage.find("timed out") != std::string::npos) {
        if (errorMessage.find("read") != std::string::npos)
            return ConnectionFailReason::READ_TIMEOUT;
        if (errorMessage.find("write") != std::string::npos)
            return ConnectionFailReason::WRITE_TIMEOUT;
        return ConnectionFailReason::CONNECT_TIMEOUT;
    }
    if (errorMessage.find("refused") != std::string::npos)
        return ConnectionFailReason::CONNECTION_REFUSED;
    if (errorMessage.find("reset") != std::string::npos)
        return ConnectionFailReason::RESET_BY_PEER;
    if (errorMessage.find("unreachable") != std::string::npos ||
        errorMessage.find("network") != std::string::npos)
        return ConnectionFailReason::NETWORK_UNREACHABLE;

    return ConnectionFailReason::UNKNOWN;
}

} // namespace progressive
