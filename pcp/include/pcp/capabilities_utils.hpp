#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CapabilitiesUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* capabilities_utils_stateToString(CapabilitiesUtilsState s);
CapabilitiesUtilsState capabilities_utils_stateFromString(const std::string& s);

struct CapabilitiesUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CapabilitiesUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CapabilitiesUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CapabilitiesUtilsStatus {
    CapabilitiesUtilsState state = CapabilitiesUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CapabilitiesUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the capabilities utils module with configuration
bool capabilities_utils_init(const CapabilitiesUtilsConfig& config);

// Handle a capabilities utils request, returning a response
CapabilitiesUtilsResponse capabilities_utils_handle(const CapabilitiesUtilsRequest& req);

// Get the current operational status
CapabilitiesUtilsStatus capabilities_utils_getStatus();

// Reset the module to initial state
void capabilities_utils_reset();

// Validate input before processing
bool capabilities_utils_validate(const std::string& input);

// Process input and return result
std::string capabilities_utils_process(const std::string& input);

// Serialize to JSON format
json capabilities_utils_toJson(const std::string& input);

// Get runtime statistics
CapabilitiesUtilsStats capabilities_utils_getStats();

// Build config from JSON
CapabilitiesUtilsConfig capabilities_utils_configFromJson(const json& j);

// Serialize config to JSON
json capabilities_utils_configToJson(const CapabilitiesUtilsConfig& config);

} // namespace pcp
