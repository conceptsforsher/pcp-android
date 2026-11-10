#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SummaryLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_summary_stateToString(SummaryLevel s);
SummaryLevel notification_summary_stateFromString(const std::string& s);

struct NotificationSummaryConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationSummaryRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationSummaryResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationSummaryStatus {
    SummaryLevel state = SummaryLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationSummaryStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification summary module with configuration
bool notification_summary_init(const NotificationSummaryConfig& config);

// Handle a notification summary request, returning a response
NotificationSummaryResponse notification_summary_handle(const NotificationSummaryRequest& req);

// Get the current operational status
NotificationSummaryStatus notification_summary_getStatus();

// Reset the module to initial state
void notification_summary_reset();

// Validate input before processing
bool notification_summary_validate(const std::string& input);

// Process input and return result
std::string notification_summary_process(const std::string& input);

// Serialize to JSON format
json notification_summary_toJson(const std::string& input);

// Get runtime statistics
NotificationSummaryStats notification_summary_getStats();

// Build config from JSON
NotificationSummaryConfig notification_summary_configFromJson(const json& j);

// Serialize config to JSON
json notification_summary_configToJson(const NotificationSummaryConfig& config);

} // namespace pcp
