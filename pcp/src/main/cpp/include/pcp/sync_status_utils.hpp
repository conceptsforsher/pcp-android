#pragma once
#include <string>
#include <cstdint>

namespace pcp {

enum class SyncState {
    Idle,
    Running,
    Paused,
    Killing,
    Killed,
    NoNetwork,
    InvalidToken
};

// Parse a sync state string to enum.
// Recognizes: "idle", "running", "paused", "killing", "killed", "no_network", "invalid_token"
SyncState parseSyncState(const std::string& stateStr);

// Check if the sync loop is actively running (running fetching data).
// True for Running; false for Idle, Paused, Killing, Killed, NoNetwork, InvalidToken.
bool isSyncRunning(SyncState state);

// Check if the sync state represents a terminal/error state.
bool isSyncTerminated(SyncState state);

// Format sync state as a human-readable string.
// e.g. "Syncing...", "Paused", "No network", "Invalid token", etc.
std::string formatSyncState(SyncState state);

// Format a detailed sync status line including progress.
// e.g. "Syncing... (42/100 events)"
// e.g. "Paused — 80/100 events fetched so far"
// e.g. "No network — reconnecting..."
std::string formatSyncProgress(SyncState state, int64_t completed, int64_t total);

// Converts a sync state enum back to its JSON string representation.
std::string syncStateToString(SyncState state);

// Get a progress fraction (0.0 to 1.0) from completed/total.
// Returns 0.0 when total is 0.
double getSyncProgress(SyncState state, int64_t completed, int64_t total);

// Format a sync error message for display.
std::string formatSyncError(SyncState state);

// Parse sync metadata from a progress JSON.
// Extracts completed/total from fields like "completed", "total".
// If afterPause is present and true, the sync is resuming after a pause.
SyncState parseSyncMetadata(const std::string& json, int64_t& outCompleted, int64_t& outTotal);

} // namespace pcp
