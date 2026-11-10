#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LifecycleUtilsPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_lifecycle_utils_stateToString(LifecycleUtilsPhase s);
LifecycleUtilsPhase session_lifecycle_utils_stateFromString(const std::string& s);

struct SessionLifecycleUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionLifecycleUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionLifecycleUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionLifecycleUtilsStatus {
    LifecycleUtilsPhase state = LifecycleUtilsPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionLifecycleUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session lifecycle utils module with configuration
bool session_lifecycle_utils_init(const SessionLifecycleUtilsConfig& config);

// Handle a session lifecycle utils request, returning a response
SessionLifecycleUtilsResponse session_lifecycle_utils_handle(const SessionLifecycleUtilsRequest& req);

// Get the current operational status
SessionLifecycleUtilsStatus session_lifecycle_utils_getStatus();

// Reset the module to initial state
void session_lifecycle_utils_reset();

// Validate input before processing
bool session_lifecycle_utils_validate(const std::string& input);

// Process input and return result
std::string session_lifecycle_utils_process(const std::string& input);

// Serialize to JSON format
json session_lifecycle_utils_toJson(const std::string& input);

// Get runtime statistics
SessionLifecycleUtilsStats session_lifecycle_utils_getStats();

// Build config from JSON
SessionLifecycleUtilsConfig session_lifecycle_utils_configFromJson(const json& j);

// Serialize config to JSON
json session_lifecycle_utils_configToJson(const SessionLifecycleUtilsConfig& config);

} // namespace pcp
