#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SchedulerPhase {
    INITIAL = 0,
    CATCHUP = 1,
    LIVE = 2,
    PAUSED = 3,
    ERROR = 4
};

const char* sync_scheduler_stateToString(SchedulerPhase s);
SchedulerPhase sync_scheduler_stateFromString(const std::string& s);

struct SyncSchedulerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SyncSchedulerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SyncSchedulerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SyncSchedulerStatus {
    SchedulerPhase state = SchedulerPhase::INITIAL;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SyncSchedulerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sync scheduler module with configuration
bool sync_scheduler_init(const SyncSchedulerConfig& config);

// Handle a sync scheduler request, returning a response
SyncSchedulerResponse sync_scheduler_handle(const SyncSchedulerRequest& req);

// Get the current operational status
SyncSchedulerStatus sync_scheduler_getStatus();

// Reset the module to initial state
void sync_scheduler_reset();

// Validate input before processing
bool sync_scheduler_validate(const std::string& input);

// Process input and return result
std::string sync_scheduler_process(const std::string& input);

// Serialize to JSON format
json sync_scheduler_toJson(const std::string& input);

// Get runtime statistics
SyncSchedulerStats sync_scheduler_getStats();

// Build config from JSON
SyncSchedulerConfig sync_scheduler_configFromJson(const json& j);

// Serialize config to JSON
json sync_scheduler_configToJson(const SyncSchedulerConfig& config);

} // namespace pcp
