#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ConfigLoaderState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* config_loader_stateToString(ConfigLoaderState s);
ConfigLoaderState config_loader_stateFromString(const std::string& s);

struct ConfigLoaderConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ConfigLoaderRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ConfigLoaderResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ConfigLoaderStatus {
    ConfigLoaderState state = ConfigLoaderState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ConfigLoaderStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the config loader module with configuration
bool config_loader_init(const ConfigLoaderConfig& config);

// Handle a config loader request, returning a response
ConfigLoaderResponse config_loader_handle(const ConfigLoaderRequest& req);

// Get the current operational status
ConfigLoaderStatus config_loader_getStatus();

// Reset the module to initial state
void config_loader_reset();

// Validate input before processing
bool config_loader_validate(const std::string& input);

// Process input and return result
std::string config_loader_process(const std::string& input);

// Serialize to JSON format
json config_loader_toJson(const std::string& input);

// Get runtime statistics
ConfigLoaderStats config_loader_getStats();

// Build config from JSON
ConfigLoaderConfig config_loader_configFromJson(const json& j);

// Serialize config to JSON
json config_loader_configToJson(const ConfigLoaderConfig& config);

} // namespace pcp
