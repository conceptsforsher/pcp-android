#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LightweightSettingsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* lightweight_settings_stateToString(LightweightSettingsState s);
LightweightSettingsState lightweight_settings_stateFromString(const std::string& s);

struct LightweightSettingsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LightweightSettingsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LightweightSettingsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LightweightSettingsStatus {
    LightweightSettingsState state = LightweightSettingsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LightweightSettingsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the lightweight settings module with configuration
bool lightweight_settings_init(const LightweightSettingsConfig& config);

// Handle a lightweight settings request, returning a response
LightweightSettingsResponse lightweight_settings_handle(const LightweightSettingsRequest& req);

// Get the current operational status
LightweightSettingsStatus lightweight_settings_getStatus();

// Reset the module to initial state
void lightweight_settings_reset();

// Validate input before processing
bool lightweight_settings_validate(const std::string& input);

// Process input and return result
std::string lightweight_settings_process(const std::string& input);

// Serialize to JSON format
json lightweight_settings_toJson(const std::string& input);

// Get runtime statistics
LightweightSettingsStats lightweight_settings_getStats();

// Build config from JSON
LightweightSettingsConfig lightweight_settings_configFromJson(const json& j);

// Serialize config to JSON
json lightweight_settings_configToJson(const LightweightSettingsConfig& config);

} // namespace pcp
