#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PowerLevelVisualizerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* power_level_visualizer_stateToString(PowerLevelVisualizerState s);
PowerLevelVisualizerState power_level_visualizer_stateFromString(const std::string& s);

struct PowerLevelVisualizerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PowerLevelVisualizerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PowerLevelVisualizerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PowerLevelVisualizerStatus {
    PowerLevelVisualizerState state = PowerLevelVisualizerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PowerLevelVisualizerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the power level visualizer module with configuration
bool power_level_visualizer_init(const PowerLevelVisualizerConfig& config);

// Handle a power level visualizer request, returning a response
PowerLevelVisualizerResponse power_level_visualizer_handle(const PowerLevelVisualizerRequest& req);

// Get the current operational status
PowerLevelVisualizerStatus power_level_visualizer_getStatus();

// Reset the module to initial state
void power_level_visualizer_reset();

// Validate input before processing
bool power_level_visualizer_validate(const std::string& input);

// Process input and return result
std::string power_level_visualizer_process(const std::string& input);

// Serialize to JSON format
json power_level_visualizer_toJson(const std::string& input);

// Get runtime statistics
PowerLevelVisualizerStats power_level_visualizer_getStats();

// Build config from JSON
PowerLevelVisualizerConfig power_level_visualizer_configFromJson(const json& j);

// Serialize config to JSON
json power_level_visualizer_configToJson(const PowerLevelVisualizerConfig& config);

} // namespace pcp
