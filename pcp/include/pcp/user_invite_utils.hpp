#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class InviteUtilsState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_invite_utils_stateToString(InviteUtilsState s);
InviteUtilsState user_invite_utils_stateFromString(const std::string& s);

struct UserInviteUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserInviteUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserInviteUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserInviteUtilsStatus {
    InviteUtilsState state = InviteUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserInviteUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user invite utils module with configuration
bool user_invite_utils_init(const UserInviteUtilsConfig& config);

// Handle a user invite utils request, returning a response
UserInviteUtilsResponse user_invite_utils_handle(const UserInviteUtilsRequest& req);

// Get the current operational status
UserInviteUtilsStatus user_invite_utils_getStatus();

// Reset the module to initial state
void user_invite_utils_reset();

// Validate input before processing
bool user_invite_utils_validate(const std::string& input);

// Process input and return result
std::string user_invite_utils_process(const std::string& input);

// Serialize to JSON format
json user_invite_utils_toJson(const std::string& input);

// Get runtime statistics
UserInviteUtilsStats user_invite_utils_getStats();

// Build config from JSON
UserInviteUtilsConfig user_invite_utils_configFromJson(const json& j);

// Serialize config to JSON
json user_invite_utils_configToJson(const UserInviteUtilsConfig& config);
