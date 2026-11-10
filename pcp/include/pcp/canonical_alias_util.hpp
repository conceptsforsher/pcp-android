#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CanonicalAliasUtilState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* canonical_alias_util_stateToString(CanonicalAliasUtilState s);
CanonicalAliasUtilState canonical_alias_util_stateFromString(const std::string& s);

struct CanonicalAliasUtilConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CanonicalAliasUtilRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CanonicalAliasUtilResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CanonicalAliasUtilStatus {
    CanonicalAliasUtilState state = CanonicalAliasUtilState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CanonicalAliasUtilStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the canonical alias util module with configuration
bool canonical_alias_util_init(const CanonicalAliasUtilConfig& config);

// Handle a canonical alias util request, returning a response
CanonicalAliasUtilResponse canonical_alias_util_handle(const CanonicalAliasUtilRequest& req);

// Get the current operational status
CanonicalAliasUtilStatus canonical_alias_util_getStatus();

// Reset the module to initial state
void canonical_alias_util_reset();

// Validate input before processing
bool canonical_alias_util_validate(const std::string& input);

// Process input and return result
std::string canonical_alias_util_process(const std::string& input);

// Serialize to JSON format
json canonical_alias_util_toJson(const std::string& input);

// Get runtime statistics
CanonicalAliasUtilStats canonical_alias_util_getStats();

// Build config from JSON
CanonicalAliasUtilConfig canonical_alias_util_configFromJson(const json& j);

// Serialize config to JSON
json canonical_alias_util_configToJson(const CanonicalAliasUtilConfig& config);

} // namespace pcp
