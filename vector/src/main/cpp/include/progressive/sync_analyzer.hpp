#ifndef PROGRESSIVE_SYNC_ANALYZER_HPP
#define PROGRESSIVE_SYNC_ANALYZER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Sync Performance Analysis ----

struct SyncEvent {
    std::string type;         // "start", "complete", "error", "timeout"
    int64_t timestampMs = 0;
    int64_t durationMs = 0;    // how long it took
    int eventsReceived = 0;
    int roomsUpdated = 0;
    std::string errorMessage;
    std::string serverName;
};

struct SyncStats {
    int totalSyncs = 0;
    int successfulSyncs = 0;
    int failedSyncs = 0;
    int timeoutSyncs = 0;
    double avgDurationMs = 0.0;
    double avgEventsPerSync = 0.0;
    double successRate = 0.0;     // 0.0-1.0
    int64_t lastSuccessfulMs = 0;
    int64_t lastSyncMs = 0;
    int totalEventsReceived = 0;
    int totalRoomsUpdated = 0;
    int64_t totalUptimeMs = 0;    // time between first and last sync
};

// Analyze sync history for performance patterns.
SyncStats analyzeSyncHistory(const std::vector<SyncEvent>& history);

// Check if the sync is healthy (no recent failures, reasonable latency).
bool isSyncHealthy(const SyncStats& stats, int64_t maxGapMs = 300000);

// Get recommended sync timeout based on history.
int suggestSyncTimeout(const SyncStats& stats);

// Format sync stats as JSON.
std::string syncStatsToJson(const SyncStats& stats);

// Format sync stats as text.
std::string syncStatsToText(const SyncStats& stats);

// ---- Initial Sync Progress ----

struct InitSyncProgress {
    int totalRooms = 0;
    int processedRooms = 0;
    int totalEvents = 0;
    int processedEvents = 0;
    int64_t startedAtMs = 0;
    int64_t estimatedRemainingMs = 0;
    double progressPercent = 0.0;
    std::string currentRoom;
    bool isComplete = false;
    bool hasError = false;
    std::string errorMessage;
};

// Update init sync progress with new data.
InitSyncProgress updateInitSyncProgress(
    InitSyncProgress current,
    int newRooms, int newEvents, const std::string& currentRoom
);

// Estimate remaining time based on processing rate.
int64_t estimateRemainingTime(const InitSyncProgress& progress);

// Format init sync progress for UI.
std::string initSyncProgressToJson(const InitSyncProgress& progress);

// Format a sync progress bar text: "[======>    ] 60%"
std::string formatProgressBar(double percent, int width = 20);

// ---- Sync Progress (streaming) ----

// Original Kotlin: SyncTask.kt — progress tracking during sync processing
struct SyncProgress {
    int totalRooms = 0;
    int processedRooms = 0;
    std::string currentRoomId;
    std::string currentStep;             // "rooms", "account_data", "presence", "to_device", "done"
    int64_t estimatedTimeRemainingMs = 0;
};

// Compute sync progress from room count and processed count.
// Returns a SyncProgress with percentage and ETA estimation.
SyncProgress computeSyncProgress(int totalRooms, int processedRooms,
                                  const std::string& currentRoomId = "",
                                  const std::string& currentStep = "");

// ---- Sync Metrics ----

// Original Kotlin: SyncTask.SyncStatisticsData — aggregated metrics for a sync cycle
struct SyncMetrics {
    int64_t startTimeMs = 0;
    int64_t endTimeMs = 0;
    int totalEvents = 0;
    int stateEvents = 0;
    int timelineEvents = 0;
    int ephemeralEvents = 0;
    int accountDataEvents = 0;
    int toDeviceEvents = 0;
    int presenceEvents = 0;
    int roomCount = 0;
    int errorCount = 0;
};

// Aggregate metrics from a parsed SyncResponse and timing data.
SyncMetrics computeSyncMetrics(const SyncResponse& response, int64_t startMs, int64_t endMs,
                                int errorCount = 0);

// Check if the initial sync has completed (all rooms processed, next_batch present).
bool isSyncComplete(const InitSyncProgress& progress);

// Calculate sync processing speed in events per second.
double getSyncSpeed(const SyncMetrics& metrics);

} // namespace progressive

#endif // PROGRESSIVE_SYNC_ANALYZER_HPP
