#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SlidingSyncUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* sliding_sync_utils_stateToString(SlidingSyncUtilsState s);
SlidingSyncUtilsState sliding_sync_utils_stateFromString(const std::string& s);

struct SlidingSyncUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SlidingSyncUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SlidingSyncUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SlidingSyncUtilsStatus {
    SlidingSyncUtilsState state = SlidingSyncUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SlidingSyncUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sliding sync utils module with configuration
bool sliding_sync_utils_init(const SlidingSyncUtilsConfig& config);

// Handle a sliding sync utils request, returning a response
SlidingSyncUtilsResponse sliding_sync_utils_handle(const SlidingSyncUtilsRequest& req);

// Get the current operational status
SlidingSyncUtilsStatus sliding_sync_utils_getStatus();

// Reset the module to initial state
void sliding_sync_utils_reset();

// Validate input before processing
bool sliding_sync_utils_validate(const std::string& input);

// Process input and return result
std::string sliding_sync_utils_process(const std::string& input);

// Serialize to JSON format
json sliding_sync_utils_toJson(const std::string& input);

// Get runtime statistics
SlidingSyncUtilsStats sliding_sync_utils_getStats();

// Build config from JSON
SlidingSyncUtilsConfig sliding_sync_utils_configFromJson(const json& j);

// Serialize config to JSON
json sliding_sync_utils_configToJson(const SlidingSyncUtilsConfig& config);

} // namespace pcp
