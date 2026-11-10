#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ActivityAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_activity_stateToString(ActivityAction s);
ActivityAction room_activity_stateFromString(const std::string& s);

struct RoomActivityConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomActivityRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomActivityResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomActivityStatus {
    ActivityAction state = ActivityAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomActivityStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room activity module with configuration
bool room_activity_init(const RoomActivityConfig& config);

// Handle a room activity request, returning a response
RoomActivityResponse room_activity_handle(const RoomActivityRequest& req);

// Get the current operational status
RoomActivityStatus room_activity_getStatus();

// Reset the module to initial state
void room_activity_reset();

// Validate input before processing
bool room_activity_validate(const std::string& input);

// Process input and return result
std::string room_activity_process(const std::string& input);

// Serialize to JSON format
json room_activity_toJson(const std::string& input);

// Get runtime statistics
RoomActivityStats room_activity_getStats();

// Build config from JSON
RoomActivityConfig room_activity_configFromJson(const json& j);

// Serialize config to JSON
json room_activity_configToJson(const RoomActivityConfig& config);

} // namespace pcp
