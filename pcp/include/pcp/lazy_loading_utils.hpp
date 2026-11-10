#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LazyLoadingUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* lazy_loading_utils_stateToString(LazyLoadingUtilsState s);
LazyLoadingUtilsState lazy_loading_utils_stateFromString(const std::string& s);

struct LazyLoadingUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LazyLoadingUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LazyLoadingUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LazyLoadingUtilsStatus {
    LazyLoadingUtilsState state = LazyLoadingUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LazyLoadingUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the lazy loading utils module with configuration
bool lazy_loading_utils_init(const LazyLoadingUtilsConfig& config);

// Handle a lazy loading utils request, returning a response
LazyLoadingUtilsResponse lazy_loading_utils_handle(const LazyLoadingUtilsRequest& req);

// Get the current operational status
LazyLoadingUtilsStatus lazy_loading_utils_getStatus();

// Reset the module to initial state
void lazy_loading_utils_reset();

// Validate input before processing
bool lazy_loading_utils_validate(const std::string& input);

// Process input and return result
std::string lazy_loading_utils_process(const std::string& input);

// Serialize to JSON format
json lazy_loading_utils_toJson(const std::string& input);

// Get runtime statistics
LazyLoadingUtilsStats lazy_loading_utils_getStats();

// Build config from JSON
LazyLoadingUtilsConfig lazy_loading_utils_configFromJson(const json& j);

// Serialize config to JSON
json lazy_loading_utils_configToJson(const LazyLoadingUtilsConfig& config);

} // namespace pcp
