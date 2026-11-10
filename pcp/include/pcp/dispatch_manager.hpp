#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DispatchManagerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* dispatch_manager_stateToString(DispatchManagerState s);
DispatchManagerState dispatch_manager_stateFromString(const std::string& s);

struct DispatchManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DispatchManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DispatchManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DispatchManagerStatus {
    DispatchManagerState state = DispatchManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DispatchManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the dispatch manager module with configuration
bool dispatch_manager_init(const DispatchManagerConfig& config);

// Handle a dispatch manager request, returning a response
DispatchManagerResponse dispatch_manager_handle(const DispatchManagerRequest& req);

// Get the current operational status
DispatchManagerStatus dispatch_manager_getStatus();

// Reset the module to initial state
void dispatch_manager_reset();

// Validate input before processing
bool dispatch_manager_validate(const std::string& input);

// Process input and return result
std::string dispatch_manager_process(const std::string& input);

// Serialize to JSON format
json dispatch_manager_toJson(const std::string& input);

// Get runtime statistics
DispatchManagerStats dispatch_manager_getStats();

// Build config from JSON
DispatchManagerConfig dispatch_manager_configFromJson(const json& j);

// Serialize config to JSON
json dispatch_manager_configToJson(const DispatchManagerConfig& config);

} // namespace pcp
