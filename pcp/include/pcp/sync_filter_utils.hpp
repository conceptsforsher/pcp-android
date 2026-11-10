#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FilterUtilsPhase {
    INITIAL = 0,
    CATCHUP = 1,
    LIVE = 2,
    PAUSED = 3,
    ERROR = 4
};

const char* sync_filter_utils_stateToString(FilterUtilsPhase s);
FilterUtilsPhase sync_filter_utils_stateFromString(const std::string& s);

struct SyncFilterUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SyncFilterUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SyncFilterUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SyncFilterUtilsStatus {
    FilterUtilsPhase state = FilterUtilsPhase::INITIAL;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SyncFilterUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sync filter utils module with configuration
bool sync_filter_utils_init(const SyncFilterUtilsConfig& config);

// Handle a sync filter utils request, returning a response
SyncFilterUtilsResponse sync_filter_utils_handle(const SyncFilterUtilsRequest& req);

// Get the current operational status
SyncFilterUtilsStatus sync_filter_utils_getStatus();

// Reset the module to initial state
void sync_filter_utils_reset();

// Validate input before processing
bool sync_filter_utils_validate(const std::string& input);

// Process input and return result
std::string sync_filter_utils_process(const std::string& input);

// Serialize to JSON format
json sync_filter_utils_toJson(const std::string& input);

// Get runtime statistics
SyncFilterUtilsStats sync_filter_utils_getStats();

// Build config from JSON
SyncFilterUtilsConfig sync_filter_utils_configFromJson(const json& j);

// Serialize config to JSON
json sync_filter_utils_configToJson(const SyncFilterUtilsConfig& config);

} // namespace pcp
