#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReactionSyncState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* reaction_sync_stateToString(ReactionSyncState s);
ReactionSyncState reaction_sync_stateFromString(const std::string& s);

struct ReactionSyncConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReactionSyncRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReactionSyncResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReactionSyncStatus {
    ReactionSyncState state = ReactionSyncState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReactionSyncStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the reaction sync module with configuration
bool reaction_sync_init(const ReactionSyncConfig& config);

// Handle a reaction sync request, returning a response
ReactionSyncResponse reaction_sync_handle(const ReactionSyncRequest& req);

// Get the current operational status
ReactionSyncStatus reaction_sync_getStatus();

// Reset the module to initial state
void reaction_sync_reset();

// Validate input before processing
bool reaction_sync_validate(const std::string& input);

// Process input and return result
std::string reaction_sync_process(const std::string& input);

// Serialize to JSON format
json reaction_sync_toJson(const std::string& input);

// Get runtime statistics
ReactionSyncStats reaction_sync_getStats();

// Build config from JSON
ReactionSyncConfig reaction_sync_configFromJson(const json& j);

// Serialize config to JSON
json reaction_sync_configToJson(const ReactionSyncConfig& config);

} // namespace pcp
