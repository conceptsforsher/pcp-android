#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class GroupLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_group_stateToString(GroupLevel s);
GroupLevel notification_group_stateFromString(const std::string& s);

struct NotificationGroupConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationGroupRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationGroupResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationGroupStatus {
    GroupLevel state = GroupLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationGroupStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification group module with configuration
bool notification_group_init(const NotificationGroupConfig& config);

// Handle a notification group request, returning a response
NotificationGroupResponse notification_group_handle(const NotificationGroupRequest& req);

// Get the current operational status
NotificationGroupStatus notification_group_getStatus();

// Reset the module to initial state
void notification_group_reset();

// Validate input before processing
bool notification_group_validate(const std::string& input);

// Process input and return result
std::string notification_group_process(const std::string& input);

// Serialize to JSON format
json notification_group_toJson(const std::string& input);

// Get runtime statistics
NotificationGroupStats notification_group_getStats();

// Build config from JSON
NotificationGroupConfig notification_group_configFromJson(const json& j);

// Serialize config to JSON
json notification_group_configToJson(const NotificationGroupConfig& config);

} // namespace pcp
