#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class KnockManagerAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_knock_manager_stateToString(KnockManagerAction s);
KnockManagerAction room_knock_manager_stateFromString(const std::string& s);

struct RoomKnockManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomKnockManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomKnockManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomKnockManagerStatus {
    KnockManagerAction state = KnockManagerAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomKnockManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room knock manager module with configuration
bool room_knock_manager_init(const RoomKnockManagerConfig& config);

// Handle a room knock manager request, returning a response
RoomKnockManagerResponse room_knock_manager_handle(const RoomKnockManagerRequest& req);

// Get the current operational status
RoomKnockManagerStatus room_knock_manager_getStatus();

// Reset the module to initial state
void room_knock_manager_reset();

// Validate input before processing
bool room_knock_manager_validate(const std::string& input);

// Process input and return result
std::string room_knock_manager_process(const std::string& input);

// Serialize to JSON format
json room_knock_manager_toJson(const std::string& input);

// Get runtime statistics
RoomKnockManagerStats room_knock_manager_getStats();

// Build config from JSON
RoomKnockManagerConfig room_knock_manager_configFromJson(const json& j);

// Serialize config to JSON
json room_knock_manager_configToJson(const RoomKnockManagerConfig& config);

} // namespace pcp
