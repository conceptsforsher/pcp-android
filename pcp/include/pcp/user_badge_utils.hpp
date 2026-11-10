#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BadgeUtilsState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_badge_utils_stateToString(BadgeUtilsState s);
BadgeUtilsState user_badge_utils_stateFromString(const std::string& s);

struct UserBadgeUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserBadgeUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserBadgeUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserBadgeUtilsStatus {
    BadgeUtilsState state = BadgeUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserBadgeUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user badge utils module with configuration
bool user_badge_utils_init(const UserBadgeUtilsConfig& config);

// Handle a user badge utils request, returning a response
UserBadgeUtilsResponse user_badge_utils_handle(const UserBadgeUtilsRequest& req);

// Get the current operational status
UserBadgeUtilsStatus user_badge_utils_getStatus();

// Reset the module to initial state
void user_badge_utils_reset();

// Validate input before processing
bool user_badge_utils_validate(const std::string& input);

// Process input and return result
std::string user_badge_utils_process(const std::string& input);

// Serialize to JSON format
json user_badge_utils_toJson(const std::string& input);

// Get runtime statistics
UserBadgeUtilsStats user_badge_utils_getStats();

// Build config from JSON
UserBadgeUtilsConfig user_badge_utils_configFromJson(const json& j);

// Serialize config to JSON
json user_badge_utils_configToJson(const UserBadgeUtilsConfig& config);

} // namespace pcp
