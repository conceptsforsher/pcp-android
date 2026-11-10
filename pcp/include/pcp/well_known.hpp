#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WellKnownState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* well_known_stateToString(WellKnownState s);
WellKnownState well_known_stateFromString(const std::string& s);

struct WellKnownConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WellKnownRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WellKnownResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WellKnownStatus {
    WellKnownState state = WellKnownState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WellKnownStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the well known module with configuration
bool well_known_init(const WellKnownConfig& config);

// Handle a well known request, returning a response
WellKnownResponse well_known_handle(const WellKnownRequest& req);

// Get the current operational status
WellKnownStatus well_known_getStatus();

// Reset the module to initial state
void well_known_reset();

// Validate input before processing
bool well_known_validate(const std::string& input);

// Process input and return result
std::string well_known_process(const std::string& input);

// Serialize to JSON format
json well_known_toJson(const std::string& input);

// Get runtime statistics
WellKnownStats well_known_getStats();

// Build config from JSON
WellKnownConfig well_known_configFromJson(const json& j);

// Serialize config to JSON
json well_known_configToJson(const WellKnownConfig& config);

} // namespace pcp
