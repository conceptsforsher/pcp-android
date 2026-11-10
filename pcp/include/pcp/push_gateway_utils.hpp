#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class GatewayUtilsKind {
    UNKNOWN = 0,
    MESSAGE = 1,
    INVITE = 2,
    CALL = 3,
    NOTIFICATION = 4
};

const char* push_gateway_utils_stateToString(GatewayUtilsKind s);
GatewayUtilsKind push_gateway_utils_stateFromString(const std::string& s);

struct PushGatewayUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PushGatewayUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PushGatewayUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PushGatewayUtilsStatus {
    GatewayUtilsKind state = GatewayUtilsKind::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PushGatewayUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the push gateway utils module with configuration
bool push_gateway_utils_init(const PushGatewayUtilsConfig& config);

// Handle a push gateway utils request, returning a response
PushGatewayUtilsResponse push_gateway_utils_handle(const PushGatewayUtilsRequest& req);

// Get the current operational status
PushGatewayUtilsStatus push_gateway_utils_getStatus();

// Reset the module to initial state
void push_gateway_utils_reset();

// Validate input before processing
bool push_gateway_utils_validate(const std::string& input);

// Process input and return result
std::string push_gateway_utils_process(const std::string& input);

// Serialize to JSON format
json push_gateway_utils_toJson(const std::string& input);

// Get runtime statistics
PushGatewayUtilsStats push_gateway_utils_getStats();

// Build config from JSON
PushGatewayUtilsConfig push_gateway_utils_configFromJson(const json& j);

// Serialize config to JSON
json push_gateway_utils_configToJson(const PushGatewayUtilsConfig& config);

} // namespace pcp
