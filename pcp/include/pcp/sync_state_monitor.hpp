#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class StateMonitorPhase {
    INITIAL = 0,
    CATCHUP = 1,
    LIVE = 2,
    PAUSED = 3,
    ERROR = 4
};

const char* sync_state_monitor_stateToString(StateMonitorPhase s);
StateMonitorPhase sync_state_monitor_stateFromString(const std::string& s);

struct SyncStateMonitorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SyncStateMonitorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SyncStateMonitorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SyncStateMonitorStatus {
    StateMonitorPhase state = StateMonitorPhase::INITIAL;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SyncStateMonitorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sync state monitor module with configuration
bool sync_state_monitor_init(const SyncStateMonitorConfig& config);

// Handle a sync state monitor request, returning a response
SyncStateMonitorResponse sync_state_monitor_handle(const SyncStateMonitorRequest& req);

// Get the current operational status
SyncStateMonitorStatus sync_state_monitor_getStatus();

// Reset the module to initial state
void sync_state_monitor_reset();

// Validate input before processing
bool sync_state_monitor_validate(const std::string& input);

// Process input and return result
std::string sync_state_monitor_process(const std::string& input);

// Serialize to JSON format
json sync_state_monitor_toJson(const std::string& input);

// Get runtime statistics
SyncStateMonitorStats sync_state_monitor_getStats();

// Build config from JSON
SyncStateMonitorConfig sync_state_monitor_configFromJson(const json& j);

// Serialize config to JSON
json sync_state_monitor_configToJson(const SyncStateMonitorConfig& config);

} // namespace pcp
