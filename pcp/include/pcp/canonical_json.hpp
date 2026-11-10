#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CanonicalJsonState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* canonical_json_stateToString(CanonicalJsonState s);
CanonicalJsonState canonical_json_stateFromString(const std::string& s);

struct CanonicalJsonConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CanonicalJsonRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CanonicalJsonResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CanonicalJsonStatus {
    CanonicalJsonState state = CanonicalJsonState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CanonicalJsonStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the canonical json module with configuration
bool canonical_json_init(const CanonicalJsonConfig& config);

// Handle a canonical json request, returning a response
CanonicalJsonResponse canonical_json_handle(const CanonicalJsonRequest& req);

// Get the current operational status
CanonicalJsonStatus canonical_json_getStatus();

// Reset the module to initial state
void canonical_json_reset();

// Validate input before processing
bool canonical_json_validate(const std::string& input);

// Process input and return result
std::string canonical_json_process(const std::string& input);

// Serialize to JSON format
json canonical_json_toJson(const std::string& input);

// Get runtime statistics
CanonicalJsonStats canonical_json_getStats();

// Build config from JSON
CanonicalJsonConfig canonical_json_configFromJson(const json& j);

// Serialize config to JSON
json canonical_json_configToJson(const CanonicalJsonConfig& config);

} // namespace pcp
