#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BatcherState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_batcher_stateToString(BatcherState s);
BatcherState event_batcher_stateFromString(const std::string& s);

struct EventBatcherConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventBatcherRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventBatcherResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventBatcherStatus {
    BatcherState state = BatcherState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventBatcherStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event batcher module with configuration
bool event_batcher_init(const EventBatcherConfig& config);

// Handle a event batcher request, returning a response
EventBatcherResponse event_batcher_handle(const EventBatcherRequest& req);

// Get the current operational status
EventBatcherStatus event_batcher_getStatus();

// Reset the module to initial state
void event_batcher_reset();

// Validate input before processing
bool event_batcher_validate(const std::string& input);

// Process input and return result
std::string event_batcher_process(const std::string& input);

// Serialize to JSON format
json event_batcher_toJson(const std::string& input);

// Get runtime statistics
EventBatcherStats event_batcher_getStats();

// Build config from JSON
EventBatcherConfig event_batcher_configFromJson(const json& j);

// Serialize config to JSON
json event_batcher_configToJson(const EventBatcherConfig& config);

} // namespace pcp
