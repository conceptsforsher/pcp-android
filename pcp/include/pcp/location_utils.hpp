#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LocationUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* location_utils_stateToString(LocationUtilsState s);
LocationUtilsState location_utils_stateFromString(const std::string& s);

struct LocationUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LocationUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LocationUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LocationUtilsStatus {
    LocationUtilsState state = LocationUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LocationUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the location utils module with configuration
bool location_utils_init(const LocationUtilsConfig& config);

// Handle a location utils request, returning a response
LocationUtilsResponse location_utils_handle(const LocationUtilsRequest& req);

// Get the current operational status
LocationUtilsStatus location_utils_getStatus();

// Reset the module to initial state
void location_utils_reset();

// Validate input before processing
bool location_utils_validate(const std::string& input);

// Process input and return result
std::string location_utils_process(const std::string& input);

// Serialize to JSON format
json location_utils_toJson(const std::string& input);

// Get runtime statistics
LocationUtilsStats location_utils_getStats();

// Build config from JSON
LocationUtilsConfig location_utils_configFromJson(const json& j);

// Serialize config to JSON
json location_utils_configToJson(const LocationUtilsConfig& config);

} // namespace pcp
