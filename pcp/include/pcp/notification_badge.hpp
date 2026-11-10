#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BadgeLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_badge_stateToString(BadgeLevel s);
BadgeLevel notification_badge_stateFromString(const std::string& s);

struct NotificationBadgeConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationBadgeRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationBadgeResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationBadgeStatus {
    BadgeLevel state = BadgeLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationBadgeStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification badge module with configuration
bool notification_badge_init(const NotificationBadgeConfig& config);

// Handle a notification badge request, returning a response
NotificationBadgeResponse notification_badge_handle(const NotificationBadgeRequest& req);

// Get the current operational status
NotificationBadgeStatus notification_badge_getStatus();

// Reset the module to initial state
void notification_badge_reset();

// Validate input before processing
bool notification_badge_validate(const std::string& input);

// Process input and return result
std::string notification_badge_process(const std::string& input);

// Serialize to JSON format
json notification_badge_toJson(const std::string& input);

// Get runtime statistics
NotificationBadgeStats notification_badge_getStats();

// Build config from JSON
NotificationBadgeConfig notification_badge_configFromJson(const json& j);

// Serialize config to JSON
json notification_badge_configToJson(const NotificationBadgeConfig& config);

} // namespace pcp
