#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PresenceMonitorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* presence_monitor_stateToString(PresenceMonitorState s);
PresenceMonitorState presence_monitor_stateFromString(const std::string& s);

struct PresenceMonitorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PresenceMonitorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PresenceMonitorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PresenceMonitorStatus {
    PresenceMonitorState state = PresenceMonitorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PresenceMonitorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the presence monitor module with configuration
bool presence_monitor_init(const PresenceMonitorConfig& config);

// Handle a presence monitor request, returning a response
PresenceMonitorResponse presence_monitor_handle(const PresenceMonitorRequest& req);

// Get the current operational status
PresenceMonitorStatus presence_monitor_getStatus();

// Reset the module to initial state
void presence_monitor_reset();

// Validate input before processing
bool presence_monitor_validate(const std::string& input);

// Process input and return result
std::string presence_monitor_process(const std::string& input);

// Serialize to JSON format
json presence_monitor_toJson(const std::string& input);

// Get runtime statistics
PresenceMonitorStats presence_monitor_getStats();

// Build config from JSON
PresenceMonitorConfig presence_monitor_configFromJson(const json& j);

// Serialize config to JSON
json presence_monitor_configToJson(const PresenceMonitorConfig& config);

} // namespace pcp
