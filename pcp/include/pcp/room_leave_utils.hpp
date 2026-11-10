#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LeaveUtilsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_leave_utils_stateToString(LeaveUtilsAction s);
LeaveUtilsAction room_leave_utils_stateFromString(const std::string& s);

struct RoomLeaveUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomLeaveUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomLeaveUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomLeaveUtilsStatus {
    LeaveUtilsAction state = LeaveUtilsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomLeaveUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room leave utils module with configuration
bool room_leave_utils_init(const RoomLeaveUtilsConfig& config);

// Handle a room leave utils request, returning a response
RoomLeaveUtilsResponse room_leave_utils_handle(const RoomLeaveUtilsRequest& req);

// Get the current operational status
RoomLeaveUtilsStatus room_leave_utils_getStatus();

// Reset the module to initial state
void room_leave_utils_reset();

// Validate input before processing
bool room_leave_utils_validate(const std::string& input);

// Process input and return result
std::string room_leave_utils_process(const std::string& input);

// Serialize to JSON format
json room_leave_utils_toJson(const std::string& input);

// Get runtime statistics
RoomLeaveUtilsStats room_leave_utils_getStats();

// Build config from JSON
RoomLeaveUtilsConfig room_leave_utils_configFromJson(const json& j);

// Serialize config to JSON
json room_leave_utils_configToJson(const RoomLeaveUtilsConfig& config);

} // namespace pcp
