#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SessionUtilsStatus {
    UNSET = 0,
    CREATING = 1,
    ACTIVE = 2,
    STALE = 3,
    CORRUPT = 4
};

const char* olm_session_utils_stateToString(SessionUtilsStatus s);
SessionUtilsStatus olm_session_utils_stateFromString(const std::string& s);

struct OlmSessionUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct OlmSessionUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct OlmSessionUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct OlmSessionUtilsStatus {
    SessionUtilsStatus state = SessionUtilsStatus::UNSET;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct OlmSessionUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the olm session utils module with configuration
bool olm_session_utils_init(const OlmSessionUtilsConfig& config);

// Handle a olm session utils request, returning a response
OlmSessionUtilsResponse olm_session_utils_handle(const OlmSessionUtilsRequest& req);

// Get the current operational status
OlmSessionUtilsStatus olm_session_utils_getStatus();

// Reset the module to initial state
void olm_session_utils_reset();

// Validate input before processing
bool olm_session_utils_validate(const std::string& input);

// Process input and return result
std::string olm_session_utils_process(const std::string& input);

// Serialize to JSON format
json olm_session_utils_toJson(const std::string& input);

// Get runtime statistics
OlmSessionUtilsStats olm_session_utils_getStats();

// Build config from JSON
OlmSessionUtilsConfig olm_session_utils_configFromJson(const json& j);

// Serialize config to JSON
json olm_session_utils_configToJson(const OlmSessionUtilsConfig& config);

} // namespace pcp
