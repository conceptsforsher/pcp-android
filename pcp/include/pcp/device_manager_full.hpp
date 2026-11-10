#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DeviceManagerFullState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* device_manager_full_stateToString(DeviceManagerFullState s);
DeviceManagerFullState device_manager_full_stateFromString(const std::string& s);

struct DeviceManagerFullConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DeviceManagerFullRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DeviceManagerFullResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DeviceManagerFullStatus {
    DeviceManagerFullState state = DeviceManagerFullState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DeviceManagerFullStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the device manager full module with configuration
bool device_manager_full_init(const DeviceManagerFullConfig& config);

// Handle a device manager full request, returning a response
DeviceManagerFullResponse device_manager_full_handle(const DeviceManagerFullRequest& req);

// Get the current operational status
DeviceManagerFullStatus device_manager_full_getStatus();

// Reset the module to initial state
void device_manager_full_reset();

// Validate input before processing
bool device_manager_full_validate(const std::string& input);

// Process input and return result
std::string device_manager_full_process(const std::string& input);

// Serialize to JSON format
json device_manager_full_toJson(const std::string& input);

// Get runtime statistics
DeviceManagerFullStats device_manager_full_getStats();

// Build config from JSON
DeviceManagerFullConfig device_manager_full_configFromJson(const json& j);

// Serialize config to JSON
json device_manager_full_configToJson(const DeviceManagerFullConfig& config);

} // namespace pcp
