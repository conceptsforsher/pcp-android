#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AccessControlAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_access_control_stateToString(AccessControlAction s);
AccessControlAction room_access_control_stateFromString(const std::string& s);

struct RoomAccessControlConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomAccessControlRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomAccessControlResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomAccessControlStatus {
    AccessControlAction state = AccessControlAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomAccessControlStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room access control module with configuration
bool room_access_control_init(const RoomAccessControlConfig& config);

// Handle a room access control request, returning a response
RoomAccessControlResponse room_access_control_handle(const RoomAccessControlRequest& req);

// Get the current operational status
RoomAccessControlStatus room_access_control_getStatus();

// Reset the module to initial state
void room_access_control_reset();

// Validate input before processing
bool room_access_control_validate(const std::string& input);

// Process input and return result
std::string room_access_control_process(const std::string& input);

// Serialize to JSON format
json room_access_control_toJson(const std::string& input);

// Get runtime statistics
RoomAccessControlStats room_access_control_getStats();

// Build config from JSON
RoomAccessControlConfig room_access_control_configFromJson(const json& j);

// Serialize config to JSON
json room_access_control_configToJson(const RoomAccessControlConfig& config);

} // namespace pcp
