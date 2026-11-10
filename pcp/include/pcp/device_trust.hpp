#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DeviceTrustState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* device_trust_stateToString(DeviceTrustState s);
DeviceTrustState device_trust_stateFromString(const std::string& s);

struct DeviceTrustConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DeviceTrustRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DeviceTrustResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DeviceTrustStatus {
    DeviceTrustState state = DeviceTrustState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DeviceTrustStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the device trust module with configuration
bool device_trust_init(const DeviceTrustConfig& config);

// Handle a device trust request, returning a response
DeviceTrustResponse device_trust_handle(const DeviceTrustRequest& req);

// Get the current operational status
DeviceTrustStatus device_trust_getStatus();

// Reset the module to initial state
void device_trust_reset();

// Validate input before processing
bool device_trust_validate(const std::string& input);

// Process input and return result
std::string device_trust_process(const std::string& input);

// Serialize to JSON format
json device_trust_toJson(const std::string& input);

// Get runtime statistics
DeviceTrustStats device_trust_getStats();

// Build config from JSON
DeviceTrustConfig device_trust_configFromJson(const json& j);

// Serialize config to JSON
json device_trust_configToJson(const DeviceTrustConfig& config);

} // namespace pcp
