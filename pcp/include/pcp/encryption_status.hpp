#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class StatusMode {
    NONE = 0,
    SETUP = 1,
    ACTIVE = 2,
    ROTATING = 3,
    ERROR = 4
};

const char* encryption_status_stateToString(StatusMode s);
StatusMode encryption_status_stateFromString(const std::string& s);

struct EncryptionStatusConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EncryptionStatusRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EncryptionStatusResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EncryptionStatusStatus {
    StatusMode state = StatusMode::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EncryptionStatusStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the encryption status module with configuration
bool encryption_status_init(const EncryptionStatusConfig& config);

// Handle a encryption status request, returning a response
EncryptionStatusResponse encryption_status_handle(const EncryptionStatusRequest& req);

// Get the current operational status
EncryptionStatusStatus encryption_status_getStatus();

// Reset the module to initial state
void encryption_status_reset();

// Validate input before processing
bool encryption_status_validate(const std::string& input);

// Process input and return result
std::string encryption_status_process(const std::string& input);

// Serialize to JSON format
json encryption_status_toJson(const std::string& input);

// Get runtime statistics
EncryptionStatusStats encryption_status_getStats();

// Build config from JSON
EncryptionStatusConfig encryption_status_configFromJson(const json& j);

// Serialize config to JSON
json encryption_status_configToJson(const EncryptionStatusConfig& config);

} // namespace pcp
