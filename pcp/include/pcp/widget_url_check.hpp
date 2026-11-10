#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WidgetUrlCheckState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* widget_url_check_stateToString(WidgetUrlCheckState s);
WidgetUrlCheckState widget_url_check_stateFromString(const std::string& s);

struct WidgetUrlCheckConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WidgetUrlCheckRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WidgetUrlCheckResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WidgetUrlCheckStatus {
    WidgetUrlCheckState state = WidgetUrlCheckState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WidgetUrlCheckStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the widget url check module with configuration
bool widget_url_check_init(const WidgetUrlCheckConfig& config);

// Handle a widget url check request, returning a response
WidgetUrlCheckResponse widget_url_check_handle(const WidgetUrlCheckRequest& req);

// Get the current operational status
WidgetUrlCheckStatus widget_url_check_getStatus();

// Reset the module to initial state
void widget_url_check_reset();

// Validate input before processing
bool widget_url_check_validate(const std::string& input);

// Process input and return result
std::string widget_url_check_process(const std::string& input);

// Serialize to JSON format
json widget_url_check_toJson(const std::string& input);

// Get runtime statistics
WidgetUrlCheckStats widget_url_check_getStats();

// Build config from JSON
WidgetUrlCheckConfig widget_url_check_configFromJson(const json& j);

// Serialize config to JSON
json widget_url_check_configToJson(const WidgetUrlCheckConfig& config);

} // namespace pcp
