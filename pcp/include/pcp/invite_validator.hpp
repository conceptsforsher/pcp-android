#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class InviteValidatorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* invite_validator_stateToString(InviteValidatorState s);
InviteValidatorState invite_validator_stateFromString(const std::string& s);

struct InviteValidatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct InviteValidatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct InviteValidatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct InviteValidatorStatus {
    InviteValidatorState state = InviteValidatorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct InviteValidatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the invite validator module with configuration
bool invite_validator_init(const InviteValidatorConfig& config);

// Handle a invite validator request, returning a response
InviteValidatorResponse invite_validator_handle(const InviteValidatorRequest& req);

// Get the current operational status
InviteValidatorStatus invite_validator_getStatus();

// Reset the module to initial state
void invite_validator_reset();

// Validate input before processing
bool invite_validator_validate(const std::string& input);

// Process input and return result
std::string invite_validator_process(const std::string& input);

// Serialize to JSON format
json invite_validator_toJson(const std::string& input);

// Get runtime statistics
InviteValidatorStats invite_validator_getStats();

// Build config from JSON
InviteValidatorConfig invite_validator_configFromJson(const json& j);

// Serialize config to JSON
json invite_validator_configToJson(const InviteValidatorConfig& config);

} // namespace pcp
