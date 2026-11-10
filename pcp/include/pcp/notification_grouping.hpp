#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class GroupingLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_grouping_stateToString(GroupingLevel s);
GroupingLevel notification_grouping_stateFromString(const std::string& s);

struct NotificationGroupingConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationGroupingRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationGroupingResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationGroupingStatus {
    GroupingLevel state = GroupingLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationGroupingStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification grouping module with configuration
bool notification_grouping_init(const NotificationGroupingConfig& config);

// Handle a notification grouping request, returning a response
NotificationGroupingResponse notification_grouping_handle(const NotificationGroupingRequest& req);

// Get the current operational status
NotificationGroupingStatus notification_grouping_getStatus();

// Reset the module to initial state
void notification_grouping_reset();

// Validate input before processing
bool notification_grouping_validate(const std::string& input);

// Process input and return result
std::string notification_grouping_process(const std::string& input);

// Serialize to JSON format
json notification_grouping_toJson(const std::string& input);

// Get runtime statistics
NotificationGroupingStats notification_grouping_getStats();

// Build config from JSON
NotificationGroupingConfig notification_grouping_configFromJson(const json& j);

// Serialize config to JSON
json notification_grouping_configToJson(const NotificationGroupingConfig& config);

} // namespace pcp
