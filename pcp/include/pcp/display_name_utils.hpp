#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DisplayNameUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* display_name_utils_stateToString(DisplayNameUtilsState s);
DisplayNameUtilsState display_name_utils_stateFromString(const std::string& s);

struct DisplayNameUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DisplayNameUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DisplayNameUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DisplayNameUtilsStatus {
    DisplayNameUtilsState state = DisplayNameUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DisplayNameUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the display name utils module with configuration
bool display_name_utils_init(const DisplayNameUtilsConfig& config);

// Handle a display name utils request, returning a response
DisplayNameUtilsResponse display_name_utils_handle(const DisplayNameUtilsRequest& req);

// Get the current operational status
DisplayNameUtilsStatus display_name_utils_getStatus();

// Reset the module to initial state
void display_name_utils_reset();

// Validate input before processing
bool display_name_utils_validate(const std::string& input);

// Process input and return result
std::string display_name_utils_process(const std::string& input);

// Serialize to JSON format
json display_name_utils_toJson(const std::string& input);

// Get runtime statistics
DisplayNameUtilsStats display_name_utils_getStats();

// Build config from JSON
DisplayNameUtilsConfig display_name_utils_configFromJson(const json& j);

// Serialize config to JSON
json display_name_utils_configToJson(const DisplayNameUtilsConfig& config);

} // namespace pcp
