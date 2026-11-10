#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class RestorePhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_restore_stateToString(RestorePhase s);
RestorePhase session_restore_stateFromString(const std::string& s);

struct SessionRestoreConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionRestoreRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionRestoreResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionRestoreStatus {
    RestorePhase state = RestorePhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionRestoreStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session restore module with configuration
bool session_restore_init(const SessionRestoreConfig& config);

// Handle a session restore request, returning a response
SessionRestoreResponse session_restore_handle(const SessionRestoreRequest& req);

// Get the current operational status
SessionRestoreStatus session_restore_getStatus();

// Reset the module to initial state
void session_restore_reset();

// Validate input before processing
bool session_restore_validate(const std::string& input);

// Process input and return result
std::string session_restore_process(const std::string& input);

// Serialize to JSON format
json session_restore_toJson(const std::string& input);

// Get runtime statistics
SessionRestoreStats session_restore_getStats();

// Build config from JSON
SessionRestoreConfig session_restore_configFromJson(const json& j);

// Serialize config to JSON
json session_restore_configToJson(const SessionRestoreConfig& config);

} // namespace pcp
