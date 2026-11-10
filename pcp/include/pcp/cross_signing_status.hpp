#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ingStatusPhase {
    UNSET = 0,
    PENDING = 1,
    BOOTSTRAPPING = 2,
    SIGNED = 3,
    VERIFIED = 4
};

const char* cross_signing_status_stateToString(ingStatusPhase s);
ingStatusPhase cross_signing_status_stateFromString(const std::string& s);

struct CrossSigningStatusConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CrossSigningStatusRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CrossSigningStatusResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CrossSigningStatusStatus {
    ingStatusPhase state = ingStatusPhase::UNSET;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CrossSigningStatusStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the cross signing status module with configuration
bool cross_signing_status_init(const CrossSigningStatusConfig& config);

// Handle a cross signing status request, returning a response
CrossSigningStatusResponse cross_signing_status_handle(const CrossSigningStatusRequest& req);

// Get the current operational status
CrossSigningStatusStatus cross_signing_status_getStatus();

// Reset the module to initial state
void cross_signing_status_reset();

// Validate input before processing
bool cross_signing_status_validate(const std::string& input);

// Process input and return result
std::string cross_signing_status_process(const std::string& input);

// Serialize to JSON format
json cross_signing_status_toJson(const std::string& input);

// Get runtime statistics
CrossSigningStatusStats cross_signing_status_getStats();

// Build config from JSON
CrossSigningStatusConfig cross_signing_status_configFromJson(const json& j);

// Serialize config to JSON
json cross_signing_status_configToJson(const CrossSigningStatusConfig& config);

} // namespace pcp
