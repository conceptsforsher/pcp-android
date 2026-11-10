#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PermissionCheckerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* permission_checker_stateToString(PermissionCheckerState s);
PermissionCheckerState permission_checker_stateFromString(const std::string& s);

struct PermissionCheckerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PermissionCheckerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PermissionCheckerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PermissionCheckerStatus {
    PermissionCheckerState state = PermissionCheckerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PermissionCheckerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the permission checker module with configuration
bool permission_checker_init(const PermissionCheckerConfig& config);

// Handle a permission checker request, returning a response
PermissionCheckerResponse permission_checker_handle(const PermissionCheckerRequest& req);

// Get the current operational status
PermissionCheckerStatus permission_checker_getStatus();

// Reset the module to initial state
void permission_checker_reset();

// Validate input before processing
bool permission_checker_validate(const std::string& input);

// Process input and return result
std::string permission_checker_process(const std::string& input);

// Serialize to JSON format
json permission_checker_toJson(const std::string& input);

// Get runtime statistics
PermissionCheckerStats permission_checker_getStats();

// Build config from JSON
PermissionCheckerConfig permission_checker_configFromJson(const json& j);

// Serialize config to JSON
json permission_checker_configToJson(const PermissionCheckerConfig& config);

} // namespace pcp
