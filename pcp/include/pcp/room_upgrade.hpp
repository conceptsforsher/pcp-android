#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UpgradeAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_upgrade_stateToString(UpgradeAction s);
UpgradeAction room_upgrade_stateFromString(const std::string& s);

struct RoomUpgradeConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomUpgradeRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomUpgradeResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomUpgradeStatus {
    UpgradeAction state = UpgradeAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomUpgradeStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room upgrade module with configuration
bool room_upgrade_init(const RoomUpgradeConfig& config);

// Handle a room upgrade request, returning a response
RoomUpgradeResponse room_upgrade_handle(const RoomUpgradeRequest& req);

// Get the current operational status
RoomUpgradeStatus room_upgrade_getStatus();

// Reset the module to initial state
void room_upgrade_reset();

// Validate input before processing
bool room_upgrade_validate(const std::string& input);

// Process input and return result
std::string room_upgrade_process(const std::string& input);

// Serialize to JSON format
json room_upgrade_toJson(const std::string& input);

// Get runtime statistics
RoomUpgradeStats room_upgrade_getStats();

// Build config from JSON
RoomUpgradeConfig room_upgrade_configFromJson(const json& j);

// Serialize config to JSON
json room_upgrade_configToJson(const RoomUpgradeConfig& config);

} // namespace pcp
