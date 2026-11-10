#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class OptimizerPhase {
    INITIAL = 0,
    CATCHUP = 1,
    LIVE = 2,
    PAUSED = 3,
    ERROR = 4
};

const char* sync_optimizer_stateToString(OptimizerPhase s);
OptimizerPhase sync_optimizer_stateFromString(const std::string& s);

struct SyncOptimizerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SyncOptimizerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SyncOptimizerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SyncOptimizerStatus {
    OptimizerPhase state = OptimizerPhase::INITIAL;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SyncOptimizerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sync optimizer module with configuration
bool sync_optimizer_init(const SyncOptimizerConfig& config);

// Handle a sync optimizer request, returning a response
SyncOptimizerResponse sync_optimizer_handle(const SyncOptimizerRequest& req);

// Get the current operational status
SyncOptimizerStatus sync_optimizer_getStatus();

// Reset the module to initial state
void sync_optimizer_reset();

// Validate input before processing
bool sync_optimizer_validate(const std::string& input);

// Process input and return result
std::string sync_optimizer_process(const std::string& input);

// Serialize to JSON format
json sync_optimizer_toJson(const std::string& input);

// Get runtime statistics
SyncOptimizerStats sync_optimizer_getStats();

// Build config from JSON
SyncOptimizerConfig sync_optimizer_configFromJson(const json& j);

// Serialize config to JSON
json sync_optimizer_configToJson(const SyncOptimizerConfig& config);

} // namespace pcp
