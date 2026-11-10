#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DeviceNamingHelperState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* device_naming_helper_stateToString(DeviceNamingHelperState s);
DeviceNamingHelperState device_naming_helper_stateFromString(const std::string& s);

struct DeviceNamingHelperConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DeviceNamingHelperRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DeviceNamingHelperResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DeviceNamingHelperStatus {
    DeviceNamingHelperState state = DeviceNamingHelperState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DeviceNamingHelperStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the device naming helper module with configuration
bool device_naming_helper_init(const DeviceNamingHelperConfig& config);

// Handle a device naming helper request, returning a response
DeviceNamingHelperResponse device_naming_helper_handle(const DeviceNamingHelperRequest& req);

// Get the current operational status
DeviceNamingHelperStatus device_naming_helper_getStatus();

// Reset the module to initial state
void device_naming_helper_reset();

// Validate input before processing
bool device_naming_helper_validate(const std::string& input);

// Process input and return result
std::string device_naming_helper_process(const std::string& input);

// Serialize to JSON format
json device_naming_helper_toJson(const std::string& input);

// Get runtime statistics
DeviceNamingHelperStats device_naming_helper_getStats();

// Build config from JSON
DeviceNamingHelperConfig device_naming_helper_configFromJson(const json& j);

// Serialize config to JSON
json device_naming_helper_configToJson(const DeviceNamingHelperConfig& config);

} // namespace pcp
