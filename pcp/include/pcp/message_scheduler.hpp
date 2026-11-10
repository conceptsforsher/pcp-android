#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SchedulerPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_scheduler_stateToString(SchedulerPhase s);
SchedulerPhase message_scheduler_stateFromString(const std::string& s);

struct MessageSchedulerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageSchedulerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageSchedulerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageSchedulerStatus {
    SchedulerPhase state = SchedulerPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageSchedulerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message scheduler module with configuration
bool message_scheduler_init(const MessageSchedulerConfig& config);

// Handle a message scheduler request, returning a response
MessageSchedulerResponse message_scheduler_handle(const MessageSchedulerRequest& req);

// Get the current operational status
MessageSchedulerStatus message_scheduler_getStatus();

// Reset the module to initial state
void message_scheduler_reset();

// Validate input before processing
bool message_scheduler_validate(const std::string& input);

// Process input and return result
std::string message_scheduler_process(const std::string& input);

// Serialize to JSON format
json message_scheduler_toJson(const std::string& input);

// Get runtime statistics
MessageSchedulerStats message_scheduler_getStats();

// Build config from JSON
MessageSchedulerConfig message_scheduler_configFromJson(const json& j);

// Serialize config to JSON
json message_scheduler_configToJson(const MessageSchedulerConfig& config);

} // namespace pcp
