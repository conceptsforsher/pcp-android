#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ContentWarningUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* content_warning_utils_stateToString(ContentWarningUtilsState s);
ContentWarningUtilsState content_warning_utils_stateFromString(const std::string& s);

struct ContentWarningUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ContentWarningUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ContentWarningUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ContentWarningUtilsStatus {
    ContentWarningUtilsState state = ContentWarningUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ContentWarningUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the content warning utils module with configuration
bool content_warning_utils_init(const ContentWarningUtilsConfig& config);

// Handle a content warning utils request, returning a response
ContentWarningUtilsResponse content_warning_utils_handle(const ContentWarningUtilsRequest& req);

// Get the current operational status
ContentWarningUtilsStatus content_warning_utils_getStatus();

// Reset the module to initial state
void content_warning_utils_reset();

// Validate input before processing
bool content_warning_utils_validate(const std::string& input);

// Process input and return result
std::string content_warning_utils_process(const std::string& input);

// Serialize to JSON format
json content_warning_utils_toJson(const std::string& input);

// Get runtime statistics
ContentWarningUtilsStats content_warning_utils_getStats();

// Build config from JSON
ContentWarningUtilsConfig content_warning_utils_configFromJson(const json& j);

// Serialize config to JSON
json content_warning_utils_configToJson(const ContentWarningUtilsConfig& config);

} // namespace pcp
