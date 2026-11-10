#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WidgetManagerExtState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* widget_manager_ext_stateToString(WidgetManagerExtState s);
WidgetManagerExtState widget_manager_ext_stateFromString(const std::string& s);

struct WidgetManagerExtConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WidgetManagerExtRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WidgetManagerExtResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WidgetManagerExtStatus {
    WidgetManagerExtState state = WidgetManagerExtState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WidgetManagerExtStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the widget manager ext module with configuration
bool widget_manager_ext_init(const WidgetManagerExtConfig& config);

// Handle a widget manager ext request, returning a response
WidgetManagerExtResponse widget_manager_ext_handle(const WidgetManagerExtRequest& req);

// Get the current operational status
WidgetManagerExtStatus widget_manager_ext_getStatus();

// Reset the module to initial state
void widget_manager_ext_reset();

// Validate input before processing
bool widget_manager_ext_validate(const std::string& input);

// Process input and return result
std::string widget_manager_ext_process(const std::string& input);

// Serialize to JSON format
json widget_manager_ext_toJson(const std::string& input);

// Get runtime statistics
WidgetManagerExtStats widget_manager_ext_getStats();

// Build config from JSON
WidgetManagerExtConfig widget_manager_ext_configFromJson(const json& j);

// Serialize config to JSON
json widget_manager_ext_configToJson(const WidgetManagerExtConfig& config);

} // namespace pcp
