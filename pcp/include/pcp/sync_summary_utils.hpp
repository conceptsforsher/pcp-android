#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class SummaryUtilsPhase {
    INITIAL = 0,
    CATCHUP = 1,
    LIVE = 2,
    PAUSED = 3,
    ERROR = 4
};

const char* sync_summary_utils_stateToString(SummaryUtilsPhase s);
SummaryUtilsPhase sync_summary_utils_stateFromString(const std::string& s);

struct SyncSummaryUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SyncSummaryUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SyncSummaryUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SyncSummaryUtilsStatus {
    SummaryUtilsPhase state = SummaryUtilsPhase::INITIAL;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SyncSummaryUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sync summary utils module with configuration
bool sync_summary_utils_init(const SyncSummaryUtilsConfig& config);

// Handle a sync summary utils request, returning a response
SyncSummaryUtilsResponse sync_summary_utils_handle(const SyncSummaryUtilsRequest& req);

// Get the current operational status
SyncSummaryUtilsStatus sync_summary_utils_getStatus();

// Reset the module to initial state
void sync_summary_utils_reset();

// Validate input before processing
bool sync_summary_utils_validate(const std::string& input);

// Process input and return result
std::string sync_summary_utils_process(const std::string& input);

// Serialize to JSON format
json sync_summary_utils_toJson(const std::string& input);

// Get runtime statistics
SyncSummaryUtilsStats sync_summary_utils_getStats();

// Build config from JSON
SyncSummaryUtilsConfig sync_summary_utils_configFromJson(const json& j);

// Serialize config to JSON
json sync_summary_utils_configToJson(const SyncSummaryUtilsConfig& config);
