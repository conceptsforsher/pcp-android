#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CustomStatusState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* custom_status_stateToString(CustomStatusState s);
CustomStatusState custom_status_stateFromString(const std::string& s);

struct CustomStatusConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CustomStatusRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CustomStatusResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CustomStatusStatus {
    CustomStatusState state = CustomStatusState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CustomStatusStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the custom status module with configuration
bool custom_status_init(const CustomStatusConfig& config);

// Handle a custom status request, returning a response
CustomStatusResponse custom_status_handle(const CustomStatusRequest& req);

// Get the current operational status
CustomStatusStatus custom_status_getStatus();

// Reset the module to initial state
void custom_status_reset();

// Validate input before processing
bool custom_status_validate(const std::string& input);

// Process input and return result
std::string custom_status_process(const std::string& input);

// Serialize to JSON format
json custom_status_toJson(const std::string& input);

// Get runtime statistics
CustomStatusStats custom_status_getStats();

// Build config from JSON
CustomStatusConfig custom_status_configFromJson(const json& j);

// Serialize config to JSON
json custom_status_configToJson(const CustomStatusConfig& config);

} // namespace pcp
