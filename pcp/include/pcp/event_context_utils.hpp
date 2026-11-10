#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ContextUtilsState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_context_utils_stateToString(ContextUtilsState s);
ContextUtilsState event_context_utils_stateFromString(const std::string& s);

struct EventContextUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventContextUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventContextUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventContextUtilsStatus {
    ContextUtilsState state = ContextUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventContextUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event context utils module with configuration
bool event_context_utils_init(const EventContextUtilsConfig& config);

// Handle a event context utils request, returning a response
EventContextUtilsResponse event_context_utils_handle(const EventContextUtilsRequest& req);

// Get the current operational status
EventContextUtilsStatus event_context_utils_getStatus();

// Reset the module to initial state
void event_context_utils_reset();

// Validate input before processing
bool event_context_utils_validate(const std::string& input);

// Process input and return result
std::string event_context_utils_process(const std::string& input);

// Serialize to JSON format
json event_context_utils_toJson(const std::string& input);

// Get runtime statistics
EventContextUtilsStats event_context_utils_getStats();

// Build config from JSON
EventContextUtilsConfig event_context_utils_configFromJson(const json& j);

// Serialize config to JSON
json event_context_utils_configToJson(const EventContextUtilsConfig& config);

} // namespace pcp
