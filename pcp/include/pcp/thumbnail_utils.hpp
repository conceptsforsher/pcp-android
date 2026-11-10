#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ThumbnailUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* thumbnail_utils_stateToString(ThumbnailUtilsState s);
ThumbnailUtilsState thumbnail_utils_stateFromString(const std::string& s);

struct ThumbnailUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ThumbnailUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ThumbnailUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ThumbnailUtilsStatus {
    ThumbnailUtilsState state = ThumbnailUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ThumbnailUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the thumbnail utils module with configuration
bool thumbnail_utils_init(const ThumbnailUtilsConfig& config);

// Handle a thumbnail utils request, returning a response
ThumbnailUtilsResponse thumbnail_utils_handle(const ThumbnailUtilsRequest& req);

// Get the current operational status
ThumbnailUtilsStatus thumbnail_utils_getStatus();

// Reset the module to initial state
void thumbnail_utils_reset();

// Validate input before processing
bool thumbnail_utils_validate(const std::string& input);

// Process input and return result
std::string thumbnail_utils_process(const std::string& input);

// Serialize to JSON format
json thumbnail_utils_toJson(const std::string& input);

// Get runtime statistics
ThumbnailUtilsStats thumbnail_utils_getStats();

// Build config from JSON
ThumbnailUtilsConfig thumbnail_utils_configFromJson(const json& j);

// Serialize config to JSON
json thumbnail_utils_configToJson(const ThumbnailUtilsConfig& config);

} // namespace pcp
