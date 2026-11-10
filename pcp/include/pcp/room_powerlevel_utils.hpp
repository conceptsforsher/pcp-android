#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PowerlevelUtilsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_powerlevel_utils_stateToString(PowerlevelUtilsAction s);
PowerlevelUtilsAction room_powerlevel_utils_stateFromString(const std::string& s);

struct RoomPowerlevelUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomPowerlevelUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomPowerlevelUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomPowerlevelUtilsStatus {
    PowerlevelUtilsAction state = PowerlevelUtilsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomPowerlevelUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room powerlevel utils module with configuration
bool room_powerlevel_utils_init(const RoomPowerlevelUtilsConfig& config);

// Handle a room powerlevel utils request, returning a response
RoomPowerlevelUtilsResponse room_powerlevel_utils_handle(const RoomPowerlevelUtilsRequest& req);

// Get the current operational status
RoomPowerlevelUtilsStatus room_powerlevel_utils_getStatus();

// Reset the module to initial state
void room_powerlevel_utils_reset();

// Validate input before processing
bool room_powerlevel_utils_validate(const std::string& input);

// Process input and return result
std::string room_powerlevel_utils_process(const std::string& input);

// Serialize to JSON format
json room_powerlevel_utils_toJson(const std::string& input);

// Get runtime statistics
RoomPowerlevelUtilsStats room_powerlevel_utils_getStats();

// Build config from JSON
RoomPowerlevelUtilsConfig room_powerlevel_utils_configFromJson(const json& j);

// Serialize config to JSON
json room_powerlevel_utils_configToJson(const RoomPowerlevelUtilsConfig& config);

} // namespace pcp
