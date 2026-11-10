#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FilterLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_filter_stateToString(FilterLevel s);
FilterLevel notification_filter_stateFromString(const std::string& s);

struct NotificationFilterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationFilterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationFilterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationFilterStatus {
    FilterLevel state = FilterLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationFilterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification filter module with configuration
bool notification_filter_init(const NotificationFilterConfig& config);

// Handle a notification filter request, returning a response
NotificationFilterResponse notification_filter_handle(const NotificationFilterRequest& req);

// Get the current operational status
NotificationFilterStatus notification_filter_getStatus();

// Reset the module to initial state
void notification_filter_reset();

// Validate input before processing
bool notification_filter_validate(const std::string& input);

// Process input and return result
std::string notification_filter_process(const std::string& input);

// Serialize to JSON format
json notification_filter_toJson(const std::string& input);

// Get runtime statistics
NotificationFilterStats notification_filter_getStats();

// Build config from JSON
NotificationFilterConfig notification_filter_configFromJson(const json& j);

// Serialize config to JSON
json notification_filter_configToJson(const NotificationFilterConfig& config);

} // namespace pcp
