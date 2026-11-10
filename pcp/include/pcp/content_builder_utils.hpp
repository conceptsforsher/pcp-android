#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ContentBuilderUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* content_builder_utils_stateToString(ContentBuilderUtilsState s);
ContentBuilderUtilsState content_builder_utils_stateFromString(const std::string& s);

struct ContentBuilderUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ContentBuilderUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ContentBuilderUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ContentBuilderUtilsStatus {
    ContentBuilderUtilsState state = ContentBuilderUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ContentBuilderUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the content builder utils module with configuration
bool content_builder_utils_init(const ContentBuilderUtilsConfig& config);

// Handle a content builder utils request, returning a response
ContentBuilderUtilsResponse content_builder_utils_handle(const ContentBuilderUtilsRequest& req);

// Get the current operational status
ContentBuilderUtilsStatus content_builder_utils_getStatus();

// Reset the module to initial state
void content_builder_utils_reset();

// Validate input before processing
bool content_builder_utils_validate(const std::string& input);

// Process input and return result
std::string content_builder_utils_process(const std::string& input);

// Serialize to JSON format
json content_builder_utils_toJson(const std::string& input);

// Get runtime statistics
ContentBuilderUtilsStats content_builder_utils_getStats();

// Build config from JSON
ContentBuilderUtilsConfig content_builder_utils_configFromJson(const json& j);

// Serialize config to JSON
json content_builder_utils_configToJson(const ContentBuilderUtilsConfig& config);

} // namespace pcp
