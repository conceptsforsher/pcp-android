#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class KeyUtilsMode {
    NONE = 0,
    SETUP = 1,
    ACTIVE = 2,
    ROTATING = 3,
    ERROR = 4
};

const char* encryption_key_utils_stateToString(KeyUtilsMode s);
KeyUtilsMode encryption_key_utils_stateFromString(const std::string& s);

struct EncryptionKeyUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EncryptionKeyUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EncryptionKeyUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EncryptionKeyUtilsStatus {
    KeyUtilsMode state = KeyUtilsMode::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EncryptionKeyUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the encryption key utils module with configuration
bool encryption_key_utils_init(const EncryptionKeyUtilsConfig& config);

// Handle a encryption key utils request, returning a response
EncryptionKeyUtilsResponse encryption_key_utils_handle(const EncryptionKeyUtilsRequest& req);

// Get the current operational status
EncryptionKeyUtilsStatus encryption_key_utils_getStatus();

// Reset the module to initial state
void encryption_key_utils_reset();

// Validate input before processing
bool encryption_key_utils_validate(const std::string& input);

// Process input and return result
std::string encryption_key_utils_process(const std::string& input);

// Serialize to JSON format
json encryption_key_utils_toJson(const std::string& input);

// Get runtime statistics
EncryptionKeyUtilsStats encryption_key_utils_getStats();

// Build config from JSON
EncryptionKeyUtilsConfig encryption_key_utils_configFromJson(const json& j);

// Serialize config to JSON
json encryption_key_utils_configToJson(const EncryptionKeyUtilsConfig& config);

} // namespace pcp
