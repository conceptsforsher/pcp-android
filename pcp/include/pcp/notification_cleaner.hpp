#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CleanerLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_cleaner_stateToString(CleanerLevel s);
CleanerLevel notification_cleaner_stateFromString(const std::string& s);

struct NotificationCleanerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationCleanerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationCleanerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationCleanerStatus {
    CleanerLevel state = CleanerLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationCleanerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification cleaner module with configuration
bool notification_cleaner_init(const NotificationCleanerConfig& config);

// Handle a notification cleaner request, returning a response
NotificationCleanerResponse notification_cleaner_handle(const NotificationCleanerRequest& req);

// Get the current operational status
NotificationCleanerStatus notification_cleaner_getStatus();

// Reset the module to initial state
void notification_cleaner_reset();

// Validate input before processing
bool notification_cleaner_validate(const std::string& input);

// Process input and return result
std::string notification_cleaner_process(const std::string& input);

// Serialize to JSON format
json notification_cleaner_toJson(const std::string& input);

// Get runtime statistics
NotificationCleanerStats notification_cleaner_getStats();

// Build config from JSON
NotificationCleanerConfig notification_cleaner_configFromJson(const json& j);

// Serialize config to JSON
json notification_cleaner_configToJson(const NotificationCleanerConfig& config);

} // namespace pcp
