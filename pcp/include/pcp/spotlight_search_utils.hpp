#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SpotlightSearchUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* spotlight_search_utils_stateToString(SpotlightSearchUtilsState s);
SpotlightSearchUtilsState spotlight_search_utils_stateFromString(const std::string& s);

struct SpotlightSearchUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SpotlightSearchUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SpotlightSearchUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SpotlightSearchUtilsStatus {
    SpotlightSearchUtilsState state = SpotlightSearchUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SpotlightSearchUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the spotlight search utils module with configuration
bool spotlight_search_utils_init(const SpotlightSearchUtilsConfig& config);

// Handle a spotlight search utils request, returning a response
SpotlightSearchUtilsResponse spotlight_search_utils_handle(const SpotlightSearchUtilsRequest& req);

// Get the current operational status
SpotlightSearchUtilsStatus spotlight_search_utils_getStatus();

// Reset the module to initial state
void spotlight_search_utils_reset();

// Validate input before processing
bool spotlight_search_utils_validate(const std::string& input);

// Process input and return result
std::string spotlight_search_utils_process(const std::string& input);

// Serialize to JSON format
json spotlight_search_utils_toJson(const std::string& input);

// Get runtime statistics
SpotlightSearchUtilsStats spotlight_search_utils_getStats();

// Build config from JSON
SpotlightSearchUtilsConfig spotlight_search_utils_configFromJson(const json& j);

// Serialize config to JSON
json spotlight_search_utils_configToJson(const SpotlightSearchUtilsConfig& config);

} // namespace pcp
