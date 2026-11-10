#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class RetentionState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_retention_stateToString(RetentionState s);
RetentionState event_retention_stateFromString(const std::string& s);

struct EventRetentionConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventRetentionRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventRetentionResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventRetentionStatus {
    RetentionState state = RetentionState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventRetentionStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event retention module with configuration
bool event_retention_init(const EventRetentionConfig& config);

// Handle a event retention request, returning a response
EventRetentionResponse event_retention_handle(const EventRetentionRequest& req);

// Get the current operational status
EventRetentionStatus event_retention_getStatus();

// Reset the module to initial state
void event_retention_reset();

// Validate input before processing
bool event_retention_validate(const std::string& input);

// Process input and return result
std::string event_retention_process(const std::string& input);

// Serialize to JSON format
json event_retention_toJson(const std::string& input);

// Get runtime statistics
EventRetentionStats event_retention_getStats();

// Build config from JSON
EventRetentionConfig event_retention_configFromJson(const json& j);

// Serialize config to JSON
json event_retention_configToJson(const EventRetentionConfig& config);

} // namespace pcp
