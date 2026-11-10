#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class InviteCounterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* invite_counter_stateToString(InviteCounterState s);
InviteCounterState invite_counter_stateFromString(const std::string& s);

struct InviteCounterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct InviteCounterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct InviteCounterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct InviteCounterStatus {
    InviteCounterState state = InviteCounterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct InviteCounterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the invite counter module with configuration
bool invite_counter_init(const InviteCounterConfig& config);

// Handle a invite counter request, returning a response
InviteCounterResponse invite_counter_handle(const InviteCounterRequest& req);

// Get the current operational status
InviteCounterStatus invite_counter_getStatus();

// Reset the module to initial state
void invite_counter_reset();

// Validate input before processing
bool invite_counter_validate(const std::string& input);

// Process input and return result
std::string invite_counter_process(const std::string& input);

// Serialize to JSON format
json invite_counter_toJson(const std::string& input);

// Get runtime statistics
InviteCounterStats invite_counter_getStats();

// Build config from JSON
InviteCounterConfig invite_counter_configFromJson(const json& j);

// Serialize config to JSON
json invite_counter_configToJson(const InviteCounterConfig& config);

} // namespace pcp
