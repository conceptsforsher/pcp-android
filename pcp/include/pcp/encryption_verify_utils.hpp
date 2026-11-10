#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class VerifyUtilsMode {
    NONE = 0,
    SETUP = 1,
    ACTIVE = 2,
    ROTATING = 3,
    ERROR = 4
};

const char* encryption_verify_utils_stateToString(VerifyUtilsMode s);
VerifyUtilsMode encryption_verify_utils_stateFromString(const std::string& s);

struct EncryptionVerifyUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EncryptionVerifyUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EncryptionVerifyUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EncryptionVerifyUtilsStatus {
    VerifyUtilsMode state = VerifyUtilsMode::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EncryptionVerifyUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the encryption verify utils module with configuration
bool encryption_verify_utils_init(const EncryptionVerifyUtilsConfig& config);

// Handle a encryption verify utils request, returning a response
EncryptionVerifyUtilsResponse encryption_verify_utils_handle(const EncryptionVerifyUtilsRequest& req);

// Get the current operational status
EncryptionVerifyUtilsStatus encryption_verify_utils_getStatus();

// Reset the module to initial state
void encryption_verify_utils_reset();

// Validate input before processing
bool encryption_verify_utils_validate(const std::string& input);

// Process input and return result
std::string encryption_verify_utils_process(const std::string& input);

// Serialize to JSON format
json encryption_verify_utils_toJson(const std::string& input);

// Get runtime statistics
EncryptionVerifyUtilsStats encryption_verify_utils_getStats();

// Build config from JSON
EncryptionVerifyUtilsConfig encryption_verify_utils_configFromJson(const json& j);

// Serialize config to JSON
json encryption_verify_utils_configToJson(const EncryptionVerifyUtilsConfig& config);

} // namespace pcp
