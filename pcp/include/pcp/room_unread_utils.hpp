#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class UnreadUtilsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_unread_utils_stateToString(UnreadUtilsAction s);
UnreadUtilsAction room_unread_utils_stateFromString(const std::string& s);

struct RoomUnreadUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomUnreadUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomUnreadUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomUnreadUtilsStatus {
    UnreadUtilsAction state = UnreadUtilsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomUnreadUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room unread utils module with configuration
bool room_unread_utils_init(const RoomUnreadUtilsConfig& config);

// Handle a room unread utils request, returning a response
RoomUnreadUtilsResponse room_unread_utils_handle(const RoomUnreadUtilsRequest& req);

// Get the current operational status
RoomUnreadUtilsStatus room_unread_utils_getStatus();

// Reset the module to initial state
void room_unread_utils_reset();

// Validate input before processing
bool room_unread_utils_validate(const std::string& input);

// Process input and return result
std::string room_unread_utils_process(const std::string& input);

// Serialize to JSON format
json room_unread_utils_toJson(const std::string& input);

// Get runtime statistics
RoomUnreadUtilsStats room_unread_utils_getStats();

// Build config from JSON
RoomUnreadUtilsConfig room_unread_utils_configFromJson(const json& j);

// Serialize config to JSON
json room_unread_utils_configToJson(const RoomUnreadUtilsConfig& config);
