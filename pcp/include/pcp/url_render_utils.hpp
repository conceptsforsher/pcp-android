#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class UrlRenderUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* url_render_utils_stateToString(UrlRenderUtilsState s);
UrlRenderUtilsState url_render_utils_stateFromString(const std::string& s);

struct UrlRenderUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UrlRenderUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UrlRenderUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UrlRenderUtilsStatus {
    UrlRenderUtilsState state = UrlRenderUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UrlRenderUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the url render utils module with configuration
bool url_render_utils_init(const UrlRenderUtilsConfig& config);

// Handle a url render utils request, returning a response
UrlRenderUtilsResponse url_render_utils_handle(const UrlRenderUtilsRequest& req);

// Get the current operational status
UrlRenderUtilsStatus url_render_utils_getStatus();

// Reset the module to initial state
void url_render_utils_reset();

// Validate input before processing
bool url_render_utils_validate(const std::string& input);

// Process input and return result
std::string url_render_utils_process(const std::string& input);

// Serialize to JSON format
json url_render_utils_toJson(const std::string& input);

// Get runtime statistics
UrlRenderUtilsStats url_render_utils_getStats();

// Build config from JSON
UrlRenderUtilsConfig url_render_utils_configFromJson(const json& j);

// Serialize config to JSON
json url_render_utils_configToJson(const UrlRenderUtilsConfig& config);
