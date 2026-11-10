#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ThreePidUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* three_pid_utils_stateToString(ThreePidUtilsState s);
ThreePidUtilsState three_pid_utils_stateFromString(const std::string& s);

struct ThreePidUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ThreePidUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ThreePidUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ThreePidUtilsStatus {
    ThreePidUtilsState state = ThreePidUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ThreePidUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the three pid utils module with configuration
bool three_pid_utils_init(const ThreePidUtilsConfig& config);

// Handle a three pid utils request, returning a response
ThreePidUtilsResponse three_pid_utils_handle(const ThreePidUtilsRequest& req);

// Get the current operational status
ThreePidUtilsStatus three_pid_utils_getStatus();

// Reset the module to initial state
void three_pid_utils_reset();

// Validate input before processing
bool three_pid_utils_validate(const std::string& input);

// Process input and return result
std::string three_pid_utils_process(const std::string& input);

// Serialize to JSON format
json three_pid_utils_toJson(const std::string& input);

// Get runtime statistics
ThreePidUtilsStats three_pid_utils_getStats();

// Build config from JSON
ThreePidUtilsConfig three_pid_utils_configFromJson(const json& j);

// Serialize config to JSON
json three_pid_utils_configToJson(const ThreePidUtilsConfig& config);

} // namespace pcp
