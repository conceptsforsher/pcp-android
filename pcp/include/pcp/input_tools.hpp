#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class InputToolsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* input_tools_stateToString(InputToolsState s);
InputToolsState input_tools_stateFromString(const std::string& s);

struct InputToolsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct InputToolsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct InputToolsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct InputToolsStatus {
    InputToolsState state = InputToolsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct InputToolsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the input tools module with configuration
bool input_tools_init(const InputToolsConfig& config);

// Handle a input tools request, returning a response
InputToolsResponse input_tools_handle(const InputToolsRequest& req);

// Get the current operational status
InputToolsStatus input_tools_getStatus();

// Reset the module to initial state
void input_tools_reset();

// Validate input before processing
bool input_tools_validate(const std::string& input);

// Process input and return result
std::string input_tools_process(const std::string& input);

// Serialize to JSON format
json input_tools_toJson(const std::string& input);

// Get runtime statistics
InputToolsStats input_tools_getStats();

// Build config from JSON
InputToolsConfig input_tools_configFromJson(const json& j);

// Serialize config to JSON
json input_tools_configToJson(const InputToolsConfig& config);

} // namespace pcp
