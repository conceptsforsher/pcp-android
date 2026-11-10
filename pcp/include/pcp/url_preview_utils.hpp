#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UrlPreviewUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* url_preview_utils_stateToString(UrlPreviewUtilsState s);
UrlPreviewUtilsState url_preview_utils_stateFromString(const std::string& s);

struct UrlPreviewUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UrlPreviewUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UrlPreviewUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UrlPreviewUtilsStatus {
    UrlPreviewUtilsState state = UrlPreviewUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UrlPreviewUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the url preview utils module with configuration
bool url_preview_utils_init(const UrlPreviewUtilsConfig& config);

// Handle a url preview utils request, returning a response
UrlPreviewUtilsResponse url_preview_utils_handle(const UrlPreviewUtilsRequest& req);

// Get the current operational status
UrlPreviewUtilsStatus url_preview_utils_getStatus();

// Reset the module to initial state
void url_preview_utils_reset();

// Validate input before processing
bool url_preview_utils_validate(const std::string& input);

// Process input and return result
std::string url_preview_utils_process(const std::string& input);

// Serialize to JSON format
json url_preview_utils_toJson(const std::string& input);

// Get runtime statistics
UrlPreviewUtilsStats url_preview_utils_getStats();

// Build config from JSON
UrlPreviewUtilsConfig url_preview_utils_configFromJson(const json& j);

// Serialize config to JSON
json url_preview_utils_configToJson(const UrlPreviewUtilsConfig& config);

} // namespace pcp
