#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class StateResolverState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* state_resolver_stateToString(StateResolverState s);
StateResolverState state_resolver_stateFromString(const std::string& s);

struct StateResolverConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct StateResolverRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct StateResolverResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct StateResolverStatus {
    StateResolverState state = StateResolverState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct StateResolverStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the state resolver module with configuration
bool state_resolver_init(const StateResolverConfig& config);

// Handle a state resolver request, returning a response
StateResolverResponse state_resolver_handle(const StateResolverRequest& req);

// Get the current operational status
StateResolverStatus state_resolver_getStatus();

// Reset the module to initial state
void state_resolver_reset();

// Validate input before processing
bool state_resolver_validate(const std::string& input);

// Process input and return result
std::string state_resolver_process(const std::string& input);

// Serialize to JSON format
json state_resolver_toJson(const std::string& input);

// Get runtime statistics
StateResolverStats state_resolver_getStats();

// Build config from JSON
StateResolverConfig state_resolver_configFromJson(const json& j);

// Serialize config to JSON
json state_resolver_configToJson(const StateResolverConfig& config);

} // namespace pcp
