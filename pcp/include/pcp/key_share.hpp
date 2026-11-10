#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ShareStatus {
    UNKNOWN = 0,
    VALID = 1,
    EXPIRED = 2,
    REVOKED = 3,
    PENDING = 4
};

const char* key_share_stateToString(ShareStatus s);
ShareStatus key_share_stateFromString(const std::string& s);

struct KeyShareConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct KeyShareRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct KeyShareResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct KeyShareStatus {
    ShareStatus state = ShareStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct KeyShareStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the key share module with configuration
bool key_share_init(const KeyShareConfig& config);

// Handle a key share request, returning a response
KeyShareResponse key_share_handle(const KeyShareRequest& req);

// Get the current operational status
KeyShareStatus key_share_getStatus();

// Reset the module to initial state
void key_share_reset();

// Validate input before processing
bool key_share_validate(const std::string& input);

// Process input and return result
std::string key_share_process(const std::string& input);

// Serialize to JSON format
json key_share_toJson(const std::string& input);

// Get runtime statistics
KeyShareStats key_share_getStats();

// Build config from JSON
KeyShareConfig key_share_configFromJson(const json& j);

// Serialize config to JSON
json key_share_configToJson(const KeyShareConfig& config);

} // namespace pcp
