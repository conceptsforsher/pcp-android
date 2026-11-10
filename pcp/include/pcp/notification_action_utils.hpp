#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ActionUtilsLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_action_utils_stateToString(ActionUtilsLevel s);
ActionUtilsLevel notification_action_utils_stateFromString(const std::string& s);

struct NotificationActionUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationActionUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationActionUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationActionUtilsStatus {
    ActionUtilsLevel state = ActionUtilsLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationActionUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification action utils module with configuration
bool notification_action_utils_init(const NotificationActionUtilsConfig& config);

// Handle a notification action utils request, returning a response
NotificationActionUtilsResponse notification_action_utils_handle(const NotificationActionUtilsRequest& req);

// Get the current operational status
NotificationActionUtilsStatus notification_action_utils_getStatus();

// Reset the module to initial state
void notification_action_utils_reset();

// Validate input before processing
bool notification_action_utils_validate(const std::string& input);

// Process input and return result
std::string notification_action_utils_process(const std::string& input);

// Serialize to JSON format
json notification_action_utils_toJson(const std::string& input);

// Get runtime statistics
NotificationActionUtilsStats notification_action_utils_getStats();

// Build config from JSON
NotificationActionUtilsConfig notification_action_utils_configFromJson(const json& j);

// Serialize config to JSON
json notification_action_utils_configToJson(const NotificationActionUtilsConfig& config);

} // namespace pcp
