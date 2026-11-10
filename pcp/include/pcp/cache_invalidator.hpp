#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CacheInvalidatorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* cache_invalidator_stateToString(CacheInvalidatorState s);
CacheInvalidatorState cache_invalidator_stateFromString(const std::string& s);

struct CacheInvalidatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CacheInvalidatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CacheInvalidatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CacheInvalidatorStatus {
    CacheInvalidatorState state = CacheInvalidatorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CacheInvalidatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the cache invalidator module with configuration
bool cache_invalidator_init(const CacheInvalidatorConfig& config);

// Handle a cache invalidator request, returning a response
CacheInvalidatorResponse cache_invalidator_handle(const CacheInvalidatorRequest& req);

// Get the current operational status
CacheInvalidatorStatus cache_invalidator_getStatus();

// Reset the module to initial state
void cache_invalidator_reset();

// Validate input before processing
bool cache_invalidator_validate(const std::string& input);

// Process input and return result
std::string cache_invalidator_process(const std::string& input);

// Serialize to JSON format
json cache_invalidator_toJson(const std::string& input);

// Get runtime statistics
CacheInvalidatorStats cache_invalidator_getStats();

// Build config from JSON
CacheInvalidatorConfig cache_invalidator_configFromJson(const json& j);

// Serialize config to JSON
json cache_invalidator_configToJson(const CacheInvalidatorConfig& config);

} // namespace pcp
