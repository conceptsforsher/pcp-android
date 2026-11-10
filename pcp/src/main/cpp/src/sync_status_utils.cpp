#include "pcp/sync_status_utils.hpp"
#include "pcp/json_parser.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {

SyncState parseSyncState(const std::string& stateStr) {
    auto s = stateStr;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s == "idle")          return SyncState::Idle;
    if (s == "running")       return SyncState::Running;
    if (s == "paused")        return SyncState::Paused;
    if (s == "killing")       return SyncState::Killing;
    if (s == "killed")        return SyncState::Killed;
    if (s == "no_network" || s == "nonetwork") return SyncState::NoNetwork;
    if (s == "invalid_token" || s == "invalidtoken") return SyncState::InvalidToken;
    return SyncState::Idle;
}

bool isSyncRunning(SyncState state) {
    return state == SyncState::Running;
}

bool isSyncTerminated(SyncState state) {
    return state == SyncState::Killing || state == SyncState::Killed ||
           state == SyncState::NoNetwork || state == SyncState::InvalidToken;
}

std::string formatSyncState(SyncState state) {
    switch (state) {
        case SyncState::Idle:    return "Idle";
        case SyncState::Running: return "Syncing...";
        case SyncState::Paused:  return "Paused";
        case SyncState::Killing: return "Stopping...";
        case SyncState::Killed:  return "Stopped";
        case SyncState::NoNetwork:   return "No network";
        case SyncState::InvalidToken: return "Invalid token";
    }
    return "Unknown";
}

std::string syncStateToString(SyncState state) {
    switch (state) {
        case SyncState::Idle:    return "idle";
        case SyncState::Running: return "running";
        case SyncState::Paused:  return "paused";
        case SyncState::Killing: return "killing";
        case SyncState::Killed:  return "killed";
        case SyncState::NoNetwork:   return "no_network";
        case SyncState::InvalidToken: return "invalid_token";
    }
    return "unknown";
}

std::string formatSyncProgress(SyncState state, int64_t completed, int64_t total) {
    std::ostringstream os;
    double pct = (total > 0) ? (static_cast<double>(completed) / static_cast<double>(total) * 100.0) : 0.0;

    switch (state) {
        case SyncState::Running:
            if (total > 0) {
                os << "Syncing... (" << completed << "/" << total << " events, " << static_cast<int>(pct) << "%)";
            } else {
                os << "Syncing... (" << completed << " events)";
            }
            break;
        case SyncState::Idle:
            os << "Up to date";
            break;
        case SyncState::Paused:
            if (total > 0) {
                os << "Paused — " << completed << "/" << total << " events fetched so far";
            } else {
                os << "Paused";
            }
            break;
        case SyncState::Killing:
            os << "Stopping sync...";
            break;
        case SyncState::Killed:
            os << "Sync stopped";
            break;
        case SyncState::NoNetwork:
            os << "No network — " << (completed > 0 ? (std::to_string(completed) + " events loaded, ") : "") << "reconnecting...";
            break;
        case SyncState::InvalidToken:
            os << "Invalid token — session expired";
            break;
    }
    return os.str();
}

double getSyncProgress(SyncState state, int64_t completed, int64_t total) {
    if (total <= 0) return (state == SyncState::Running) ? 0.0 : 1.0;
    double fraction = static_cast<double>(completed) / static_cast<double>(total);
    if (fraction < 0.0) return 0.0;
    if (fraction > 1.0) return 1.0;
    return fraction;
}

std::string formatSyncError(SyncState state) {
    switch (state) {
        case SyncState::NoNetwork:   return "Connection lost. Please check your network and try again.";
        case SyncState::InvalidToken: return "Your session has expired. Please sign in again.";
        case SyncState::Killed:      return "Sync was stopped. Tap to reconnect.";
        default: return "";
    }
}

SyncState parseSyncMetadata(const std::string& json, int64_t& outCompleted, int64_t& outTotal) {
    outCompleted = parseJsonInt64Value(json, "completed", 0);
    outTotal     = parseJsonInt64Value(json, "total", 0);

    bool afterPause = parseJsonBoolValue(json, "after_pause", false);

    auto stateStr = parseJsonStringValue(json, "state");
    SyncState state = stateStr.empty() ? SyncState::Running : parseSyncState(stateStr);

    if (state == SyncState::Running && afterPause && outCompleted == 0 && outTotal > 0) {
        state = SyncState::Paused;
    }

    return state;
}

} // namespace pcp
