#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class IgnoreUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* ignore_utils_stateToString(IgnoreUtilsState s);
IgnoreUtilsState ignore_utils_stateFromString(const std::string& s);

struct IgnoreUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct IgnoreUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct IgnoreUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct IgnoreUtilsStatus {
    IgnoreUtilsState state = IgnoreUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct IgnoreUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the ignore utils module with configuration
bool ignore_utils_init(const IgnoreUtilsConfig& config);

// Handle a ignore utils request, returning a response
IgnoreUtilsResponse ignore_utils_handle(const IgnoreUtilsRequest& req);

// Get the current operational status
IgnoreUtilsStatus ignore_utils_getStatus();

// Reset the module to initial state
void ignore_utils_reset();

// Validate input before processing
bool ignore_utils_validate(const std::string& input);

// Process input and return result
std::string ignore_utils_process(const std::string& input);

// Serialize to JSON format
json ignore_utils_toJson(const std::string& input);

// Get runtime statistics
IgnoreUtilsStats ignore_utils_getStats();

// Build config from JSON
IgnoreUtilsConfig ignore_utils_configFromJson(const json& j);

// Serialize config to JSON
json ignore_utils_configToJson(const IgnoreUtilsConfig& config);

} // namespace pcp
