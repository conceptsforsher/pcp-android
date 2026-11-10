#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AvatarUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* avatar_utils_stateToString(AvatarUtilsState s);
AvatarUtilsState avatar_utils_stateFromString(const std::string& s);

struct AvatarUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AvatarUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AvatarUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AvatarUtilsStatus {
    AvatarUtilsState state = AvatarUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AvatarUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the avatar utils module with configuration
bool avatar_utils_init(const AvatarUtilsConfig& config);

// Handle a avatar utils request, returning a response
AvatarUtilsResponse avatar_utils_handle(const AvatarUtilsRequest& req);

// Get the current operational status
AvatarUtilsStatus avatar_utils_getStatus();

// Reset the module to initial state
void avatar_utils_reset();

// Validate input before processing
bool avatar_utils_validate(const std::string& input);

// Process input and return result
std::string avatar_utils_process(const std::string& input);

// Serialize to JSON format
json avatar_utils_toJson(const std::string& input);

// Get runtime statistics
AvatarUtilsStats avatar_utils_getStats();

// Build config from JSON
AvatarUtilsConfig avatar_utils_configFromJson(const json& j);

// Serialize config to JSON
json avatar_utils_configToJson(const AvatarUtilsConfig& config);

} // namespace pcp
