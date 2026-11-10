#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BlockManagerState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_block_manager_stateToString(BlockManagerState s);
BlockManagerState user_block_manager_stateFromString(const std::string& s);

struct UserBlockManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserBlockManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserBlockManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserBlockManagerStatus {
    BlockManagerState state = BlockManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserBlockManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user block manager module with configuration
bool user_block_manager_init(const UserBlockManagerConfig& config);

// Handle a user block manager request, returning a response
UserBlockManagerResponse user_block_manager_handle(const UserBlockManagerRequest& req);

// Get the current operational status
UserBlockManagerStatus user_block_manager_getStatus();

// Reset the module to initial state
void user_block_manager_reset();

// Validate input before processing
bool user_block_manager_validate(const std::string& input);

// Process input and return result
std::string user_block_manager_process(const std::string& input);

// Serialize to JSON format
json user_block_manager_toJson(const std::string& input);

// Get runtime statistics
UserBlockManagerStats user_block_manager_getStats();

// Build config from JSON
UserBlockManagerConfig user_block_manager_configFromJson(const json& j);

// Serialize config to JSON
json user_block_manager_configToJson(const UserBlockManagerConfig& config);

} // namespace pcp
