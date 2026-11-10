#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PresenceSyncState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* presence_sync_stateToString(PresenceSyncState s);
PresenceSyncState presence_sync_stateFromString(const std::string& s);

struct PresenceSyncConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PresenceSyncRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PresenceSyncResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PresenceSyncStatus {
    PresenceSyncState state = PresenceSyncState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PresenceSyncStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the presence sync module with configuration
bool presence_sync_init(const PresenceSyncConfig& config);

// Handle a presence sync request, returning a response
PresenceSyncResponse presence_sync_handle(const PresenceSyncRequest& req);

// Get the current operational status
PresenceSyncStatus presence_sync_getStatus();

// Reset the module to initial state
void presence_sync_reset();

// Validate input before processing
bool presence_sync_validate(const std::string& input);

// Process input and return result
std::string presence_sync_process(const std::string& input);

// Serialize to JSON format
json presence_sync_toJson(const std::string& input);

// Get runtime statistics
PresenceSyncStats presence_sync_getStats();

// Build config from JSON
PresenceSyncConfig presence_sync_configFromJson(const json& j);

// Serialize config to JSON
json presence_sync_configToJson(const PresenceSyncConfig& config);

} // namespace pcp
