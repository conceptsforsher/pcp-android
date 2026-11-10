#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ForwardUtilsState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_forward_utils_stateToString(ForwardUtilsState s);
ForwardUtilsState event_forward_utils_stateFromString(const std::string& s);

struct EventForwardUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventForwardUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventForwardUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventForwardUtilsStatus {
    ForwardUtilsState state = ForwardUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventForwardUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event forward utils module with configuration
bool event_forward_utils_init(const EventForwardUtilsConfig& config);

// Handle a event forward utils request, returning a response
EventForwardUtilsResponse event_forward_utils_handle(const EventForwardUtilsRequest& req);

// Get the current operational status
EventForwardUtilsStatus event_forward_utils_getStatus();

// Reset the module to initial state
void event_forward_utils_reset();

// Validate input before processing
bool event_forward_utils_validate(const std::string& input);

// Process input and return result
std::string event_forward_utils_process(const std::string& input);

// Serialize to JSON format
json event_forward_utils_toJson(const std::string& input);

// Get runtime statistics
EventForwardUtilsStats event_forward_utils_getStats();

// Build config from JSON
EventForwardUtilsConfig event_forward_utils_configFromJson(const json& j);

// Serialize config to JSON
json event_forward_utils_configToJson(const EventForwardUtilsConfig& config);
