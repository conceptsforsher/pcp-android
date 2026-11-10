#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WidgetApiUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* widget_api_utils_stateToString(WidgetApiUtilsState s);
WidgetApiUtilsState widget_api_utils_stateFromString(const std::string& s);

struct WidgetApiUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WidgetApiUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WidgetApiUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WidgetApiUtilsStatus {
    WidgetApiUtilsState state = WidgetApiUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WidgetApiUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the widget api utils module with configuration
bool widget_api_utils_init(const WidgetApiUtilsConfig& config);

// Handle a widget api utils request, returning a response
WidgetApiUtilsResponse widget_api_utils_handle(const WidgetApiUtilsRequest& req);

// Get the current operational status
WidgetApiUtilsStatus widget_api_utils_getStatus();

// Reset the module to initial state
void widget_api_utils_reset();

// Validate input before processing
bool widget_api_utils_validate(const std::string& input);

// Process input and return result
std::string widget_api_utils_process(const std::string& input);

// Serialize to JSON format
json widget_api_utils_toJson(const std::string& input);

// Get runtime statistics
WidgetApiUtilsStats widget_api_utils_getStats();

// Build config from JSON
WidgetApiUtilsConfig widget_api_utils_configFromJson(const json& j);

// Serialize config to JSON
json widget_api_utils_configToJson(const WidgetApiUtilsConfig& config);

} // namespace pcp
