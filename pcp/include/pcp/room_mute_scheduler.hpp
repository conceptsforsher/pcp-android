#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MuteSchedulerAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_mute_scheduler_stateToString(MuteSchedulerAction s);
MuteSchedulerAction room_mute_scheduler_stateFromString(const std::string& s);

struct RoomMuteSchedulerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomMuteSchedulerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomMuteSchedulerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomMuteSchedulerStatus {
    MuteSchedulerAction state = MuteSchedulerAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomMuteSchedulerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room mute scheduler module with configuration
bool room_mute_scheduler_init(const RoomMuteSchedulerConfig& config);

// Handle a room mute scheduler request, returning a response
RoomMuteSchedulerResponse room_mute_scheduler_handle(const RoomMuteSchedulerRequest& req);

// Get the current operational status
RoomMuteSchedulerStatus room_mute_scheduler_getStatus();

// Reset the module to initial state
void room_mute_scheduler_reset();

// Validate input before processing
bool room_mute_scheduler_validate(const std::string& input);

// Process input and return result
std::string room_mute_scheduler_process(const std::string& input);

// Serialize to JSON format
json room_mute_scheduler_toJson(const std::string& input);

// Get runtime statistics
RoomMuteSchedulerStats room_mute_scheduler_getStats();

// Build config from JSON
RoomMuteSchedulerConfig room_mute_scheduler_configFromJson(const json& j);

// Serialize config to JSON
json room_mute_scheduler_configToJson(const RoomMuteSchedulerConfig& config);

} // namespace pcp
