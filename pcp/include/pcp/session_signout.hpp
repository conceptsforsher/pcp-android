#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SignoutPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_signout_stateToString(SignoutPhase s);
SignoutPhase session_signout_stateFromString(const std::string& s);

struct SessionSignoutConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionSignoutRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionSignoutResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionSignoutStatus {
    SignoutPhase state = SignoutPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionSignoutStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session signout module with configuration
bool session_signout_init(const SessionSignoutConfig& config);

// Handle a session signout request, returning a response
SessionSignoutResponse session_signout_handle(const SessionSignoutRequest& req);

// Get the current operational status
SessionSignoutStatus session_signout_getStatus();

// Reset the module to initial state
void session_signout_reset();

// Validate input before processing
bool session_signout_validate(const std::string& input);

// Process input and return result
std::string session_signout_process(const std::string& input);

// Serialize to JSON format
json session_signout_toJson(const std::string& input);

// Get runtime statistics
SessionSignoutStats session_signout_getStats();

// Build config from JSON
SessionSignoutConfig session_signout_configFromJson(const json& j);

// Serialize config to JSON
json session_signout_configToJson(const SessionSignoutConfig& config);

} // namespace pcp
