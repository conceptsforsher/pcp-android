#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ServerAclUtilsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_server_acl_utils_stateToString(ServerAclUtilsAction s);
ServerAclUtilsAction room_server_acl_utils_stateFromString(const std::string& s);

struct RoomServerAclUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomServerAclUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomServerAclUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomServerAclUtilsStatus {
    ServerAclUtilsAction state = ServerAclUtilsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomServerAclUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room server acl utils module with configuration
bool room_server_acl_utils_init(const RoomServerAclUtilsConfig& config);

// Handle a room server acl utils request, returning a response
RoomServerAclUtilsResponse room_server_acl_utils_handle(const RoomServerAclUtilsRequest& req);

// Get the current operational status
RoomServerAclUtilsStatus room_server_acl_utils_getStatus();

// Reset the module to initial state
void room_server_acl_utils_reset();

// Validate input before processing
bool room_server_acl_utils_validate(const std::string& input);

// Process input and return result
std::string room_server_acl_utils_process(const std::string& input);

// Serialize to JSON format
json room_server_acl_utils_toJson(const std::string& input);

// Get runtime statistics
RoomServerAclUtilsStats room_server_acl_utils_getStats();

// Build config from JSON
RoomServerAclUtilsConfig room_server_acl_utils_configFromJson(const json& j);

// Serialize config to JSON
json room_server_acl_utils_configToJson(const RoomServerAclUtilsConfig& config);
