#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WatchdogPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_watchdog_stateToString(WatchdogPhase s);
WatchdogPhase session_watchdog_stateFromString(const std::string& s);

struct SessionWatchdogConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionWatchdogRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionWatchdogResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionWatchdogStatus {
    WatchdogPhase state = WatchdogPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionWatchdogStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session watchdog module with configuration
bool session_watchdog_init(const SessionWatchdogConfig& config);

// Handle a session watchdog request, returning a response
SessionWatchdogResponse session_watchdog_handle(const SessionWatchdogRequest& req);

// Get the current operational status
SessionWatchdogStatus session_watchdog_getStatus();

// Reset the module to initial state
void session_watchdog_reset();

// Validate input before processing
bool session_watchdog_validate(const std::string& input);

// Process input and return result
std::string session_watchdog_process(const std::string& input);

// Serialize to JSON format
json session_watchdog_toJson(const std::string& input);

// Get runtime statistics
SessionWatchdogStats session_watchdog_getStats();

// Build config from JSON
SessionWatchdogConfig session_watchdog_configFromJson(const json& j);

// Serialize config to JSON
json session_watchdog_configToJson(const SessionWatchdogConfig& config);

} // namespace pcp
