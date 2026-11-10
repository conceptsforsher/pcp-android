#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class HashState {
    UNKNOWN = 0,
    UNVERIFIED = 1,
    PENDING = 2,
    VERIFIED = 3,
    REVOKED = 4
};

const char* identity_hash_stateToString(HashState s);
HashState identity_hash_stateFromString(const std::string& s);

struct IdentityHashConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct IdentityHashRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct IdentityHashResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct IdentityHashStatus {
    HashState state = HashState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct IdentityHashStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the identity hash module with configuration
bool identity_hash_init(const IdentityHashConfig& config);

// Handle a identity hash request, returning a response
IdentityHashResponse identity_hash_handle(const IdentityHashRequest& req);

// Get the current operational status
IdentityHashStatus identity_hash_getStatus();

// Reset the module to initial state
void identity_hash_reset();

// Validate input before processing
bool identity_hash_validate(const std::string& input);

// Process input and return result
std::string identity_hash_process(const std::string& input);

// Serialize to JSON format
json identity_hash_toJson(const std::string& input);

// Get runtime statistics
IdentityHashStats identity_hash_getStats();

// Build config from JSON
IdentityHashConfig identity_hash_configFromJson(const json& j);

// Serialize config to JSON
json identity_hash_configToJson(const IdentityHashConfig& config);

} // namespace pcp
