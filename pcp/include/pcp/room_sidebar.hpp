#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SidebarAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_sidebar_stateToString(SidebarAction s);
SidebarAction room_sidebar_stateFromString(const std::string& s);

struct RoomSidebarConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomSidebarRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomSidebarResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomSidebarStatus {
    SidebarAction state = SidebarAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomSidebarStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room sidebar module with configuration
bool room_sidebar_init(const RoomSidebarConfig& config);

// Handle a room sidebar request, returning a response
RoomSidebarResponse room_sidebar_handle(const RoomSidebarRequest& req);

// Get the current operational status
RoomSidebarStatus room_sidebar_getStatus();

// Reset the module to initial state
void room_sidebar_reset();

// Validate input before processing
bool room_sidebar_validate(const std::string& input);

// Process input and return result
std::string room_sidebar_process(const std::string& input);

// Serialize to JSON format
json room_sidebar_toJson(const std::string& input);

// Get runtime statistics
RoomSidebarStats room_sidebar_getStats();

// Build config from JSON
RoomSidebarConfig room_sidebar_configFromJson(const json& j);

// Serialize config to JSON
json room_sidebar_configToJson(const RoomSidebarConfig& config);

} // namespace pcp
