#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SlidingSyncState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* sliding_sync_stateToString(SlidingSyncState s);
SlidingSyncState sliding_sync_stateFromString(const std::string& s);

struct SlidingSyncConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SlidingSyncRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SlidingSyncResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SlidingSyncStatus {
    SlidingSyncState state = SlidingSyncState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SlidingSyncStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sliding sync module with configuration
bool sliding_sync_init(const SlidingSyncConfig& config);

// Handle a sliding sync request, returning a response
SlidingSyncResponse sliding_sync_handle(const SlidingSyncRequest& req);

// Get the current operational status
SlidingSyncStatus sliding_sync_getStatus();

// Reset the module to initial state
void sliding_sync_reset();

// Validate input before processing
bool sliding_sync_validate(const std::string& input);

// Process input and return result
std::string sliding_sync_process(const std::string& input);

// Serialize to JSON format
json sliding_sync_toJson(const std::string& input);

// Get runtime statistics
SlidingSyncStats sliding_sync_getStats();

// Build config from JSON
SlidingSyncConfig sliding_sync_configFromJson(const json& j);

// Serialize config to JSON
json sliding_sync_configToJson(const SlidingSyncConfig& config);

} // namespace pcp
