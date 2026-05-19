#include "progressive/connection_monitor.hpp"
#include <sstream>
#include <chrono>

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

} // namespace progressive
