#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BackgroundSchedulerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* background_scheduler_stateToString(BackgroundSchedulerState s);
BackgroundSchedulerState background_scheduler_stateFromString(const std::string& s);

struct BackgroundSchedulerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct BackgroundSchedulerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct BackgroundSchedulerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct BackgroundSchedulerStatus {
    BackgroundSchedulerState state = BackgroundSchedulerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct BackgroundSchedulerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the background scheduler module with configuration
bool background_scheduler_init(const BackgroundSchedulerConfig& config);

// Handle a background scheduler request, returning a response
BackgroundSchedulerResponse background_scheduler_handle(const BackgroundSchedulerRequest& req);

// Get the current operational status
BackgroundSchedulerStatus background_scheduler_getStatus();

// Reset the module to initial state
void background_scheduler_reset();

// Validate input before processing
bool background_scheduler_validate(const std::string& input);

// Process input and return result
std::string background_scheduler_process(const std::string& input);

// Serialize to JSON format
json background_scheduler_toJson(const std::string& input);

// Get runtime statistics
BackgroundSchedulerStats background_scheduler_getStats();

// Build config from JSON
BackgroundSchedulerConfig background_scheduler_configFromJson(const json& j);

// Serialize config to JSON
json background_scheduler_configToJson(const BackgroundSchedulerConfig& config);

} // namespace pcp
