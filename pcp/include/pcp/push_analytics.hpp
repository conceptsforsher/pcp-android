#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AnalyticsKind {
    UNKNOWN = 0,
    MESSAGE = 1,
    INVITE = 2,
    CALL = 3,
    NOTIFICATION = 4
};

const char* push_analytics_stateToString(AnalyticsKind s);
AnalyticsKind push_analytics_stateFromString(const std::string& s);

struct PushAnalyticsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PushAnalyticsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PushAnalyticsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PushAnalyticsStatus {
    AnalyticsKind state = AnalyticsKind::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PushAnalyticsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the push analytics module with configuration
bool push_analytics_init(const PushAnalyticsConfig& config);

// Handle a push analytics request, returning a response
PushAnalyticsResponse push_analytics_handle(const PushAnalyticsRequest& req);

// Get the current operational status
PushAnalyticsStatus push_analytics_getStatus();

// Reset the module to initial state
void push_analytics_reset();

// Validate input before processing
bool push_analytics_validate(const std::string& input);

// Process input and return result
std::string push_analytics_process(const std::string& input);

// Serialize to JSON format
json push_analytics_toJson(const std::string& input);

// Get runtime statistics
PushAnalyticsStats push_analytics_getStats();

// Build config from JSON
PushAnalyticsConfig push_analytics_configFromJson(const json& j);

// Serialize config to JSON
json push_analytics_configToJson(const PushAnalyticsConfig& config);

} // namespace pcp
