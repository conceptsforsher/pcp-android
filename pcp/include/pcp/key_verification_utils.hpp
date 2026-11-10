#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class VerificationUtilsStatus {
    UNKNOWN = 0,
    VALID = 1,
    EXPIRED = 2,
    REVOKED = 3,
    PENDING = 4
};

const char* key_verification_utils_stateToString(VerificationUtilsStatus s);
VerificationUtilsStatus key_verification_utils_stateFromString(const std::string& s);

struct KeyVerificationUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct KeyVerificationUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct KeyVerificationUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct KeyVerificationUtilsStatus {
    VerificationUtilsStatus state = VerificationUtilsStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct KeyVerificationUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the key verification utils module with configuration
bool key_verification_utils_init(const KeyVerificationUtilsConfig& config);

// Handle a key verification utils request, returning a response
KeyVerificationUtilsResponse key_verification_utils_handle(const KeyVerificationUtilsRequest& req);

// Get the current operational status
KeyVerificationUtilsStatus key_verification_utils_getStatus();

// Reset the module to initial state
void key_verification_utils_reset();

// Validate input before processing
bool key_verification_utils_validate(const std::string& input);

// Process input and return result
std::string key_verification_utils_process(const std::string& input);

// Serialize to JSON format
json key_verification_utils_toJson(const std::string& input);

// Get runtime statistics
KeyVerificationUtilsStats key_verification_utils_getStats();

// Build config from JSON
KeyVerificationUtilsConfig key_verification_utils_configFromJson(const json& j);

// Serialize config to JSON
json key_verification_utils_configToJson(const KeyVerificationUtilsConfig& config);

} // namespace pcp
