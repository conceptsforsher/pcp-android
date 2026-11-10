#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MaskState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_mask_stateToString(MaskState s);
MaskState user_mask_stateFromString(const std::string& s);

struct UserMaskConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserMaskRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserMaskResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserMaskStatus {
    MaskState state = MaskState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserMaskStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user mask module with configuration
bool user_mask_init(const UserMaskConfig& config);

// Handle a user mask request, returning a response
UserMaskResponse user_mask_handle(const UserMaskRequest& req);

// Get the current operational status
UserMaskStatus user_mask_getStatus();

// Reset the module to initial state
void user_mask_reset();

// Validate input before processing
bool user_mask_validate(const std::string& input);

// Process input and return result
std::string user_mask_process(const std::string& input);

// Serialize to JSON format
json user_mask_toJson(const std::string& input);

// Get runtime statistics
UserMaskStats user_mask_getStats();

// Build config from JSON
UserMaskConfig user_mask_configFromJson(const json& j);

// Serialize config to JSON
json user_mask_configToJson(const UserMaskConfig& config);

} // namespace pcp
