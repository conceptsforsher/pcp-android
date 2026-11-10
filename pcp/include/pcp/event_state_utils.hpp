#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class StateUtilsState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_state_utils_stateToString(StateUtilsState s);
StateUtilsState event_state_utils_stateFromString(const std::string& s);

struct EventStateUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventStateUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventStateUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventStateUtilsStatus {
    StateUtilsState state = StateUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventStateUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event state utils module with configuration
bool event_state_utils_init(const EventStateUtilsConfig& config);

// Handle a event state utils request, returning a response
EventStateUtilsResponse event_state_utils_handle(const EventStateUtilsRequest& req);

// Get the current operational status
EventStateUtilsStatus event_state_utils_getStatus();

// Reset the module to initial state
void event_state_utils_reset();

// Validate input before processing
bool event_state_utils_validate(const std::string& input);

// Process input and return result
std::string event_state_utils_process(const std::string& input);

// Serialize to JSON format
json event_state_utils_toJson(const std::string& input);

// Get runtime statistics
EventStateUtilsStats event_state_utils_getStats();

// Build config from JSON
EventStateUtilsConfig event_state_utils_configFromJson(const json& j);

// Serialize config to JSON
json event_state_utils_configToJson(const EventStateUtilsConfig& config);

} // namespace pcp
