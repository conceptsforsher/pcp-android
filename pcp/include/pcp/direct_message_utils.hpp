#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class DirectMessageUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* direct_message_utils_stateToString(DirectMessageUtilsState s);
DirectMessageUtilsState direct_message_utils_stateFromString(const std::string& s);

struct DirectMessageUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DirectMessageUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DirectMessageUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DirectMessageUtilsStatus {
    DirectMessageUtilsState state = DirectMessageUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DirectMessageUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the direct message utils module with configuration
bool direct_message_utils_init(const DirectMessageUtilsConfig& config);

// Handle a direct message utils request, returning a response
DirectMessageUtilsResponse direct_message_utils_handle(const DirectMessageUtilsRequest& req);

// Get the current operational status
DirectMessageUtilsStatus direct_message_utils_getStatus();

// Reset the module to initial state
void direct_message_utils_reset();

// Validate input before processing
bool direct_message_utils_validate(const std::string& input);

// Process input and return result
std::string direct_message_utils_process(const std::string& input);

// Serialize to JSON format
json direct_message_utils_toJson(const std::string& input);

// Get runtime statistics
DirectMessageUtilsStats direct_message_utils_getStats();

// Build config from JSON
DirectMessageUtilsConfig direct_message_utils_configFromJson(const json& j);

// Serialize config to JSON
json direct_message_utils_configToJson(const DirectMessageUtilsConfig& config);
