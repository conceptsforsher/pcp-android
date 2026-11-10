#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class VersionCheckStatus {
    UNKNOWN = 0,
    AVAILABLE = 1,
    DEGRADED = 2,
    UNAVAILABLE = 3,
    MAINTENANCE = 4
};

const char* server_version_check_stateToString(VersionCheckStatus s);
VersionCheckStatus server_version_check_stateFromString(const std::string& s);

struct ServerVersionCheckConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ServerVersionCheckRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ServerVersionCheckResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ServerVersionCheckStatus {
    VersionCheckStatus state = VersionCheckStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ServerVersionCheckStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the server version check module with configuration
bool server_version_check_init(const ServerVersionCheckConfig& config);

// Handle a server version check request, returning a response
ServerVersionCheckResponse server_version_check_handle(const ServerVersionCheckRequest& req);

// Get the current operational status
ServerVersionCheckStatus server_version_check_getStatus();

// Reset the module to initial state
void server_version_check_reset();

// Validate input before processing
bool server_version_check_validate(const std::string& input);

// Process input and return result
std::string server_version_check_process(const std::string& input);

// Serialize to JSON format
json server_version_check_toJson(const std::string& input);

// Get runtime statistics
ServerVersionCheckStats server_version_check_getStats();

// Build config from JSON
ServerVersionCheckConfig server_version_check_configFromJson(const json& j);

// Serialize config to JSON
json server_version_check_configToJson(const ServerVersionCheckConfig& config);

} // namespace pcp
