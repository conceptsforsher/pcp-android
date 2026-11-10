#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class AvatarFallbackUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* avatar_fallback_utils_stateToString(AvatarFallbackUtilsState s);
AvatarFallbackUtilsState avatar_fallback_utils_stateFromString(const std::string& s);

struct AvatarFallbackUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AvatarFallbackUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AvatarFallbackUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AvatarFallbackUtilsStatus {
    AvatarFallbackUtilsState state = AvatarFallbackUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AvatarFallbackUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the avatar fallback utils module with configuration
bool avatar_fallback_utils_init(const AvatarFallbackUtilsConfig& config);

// Handle a avatar fallback utils request, returning a response
AvatarFallbackUtilsResponse avatar_fallback_utils_handle(const AvatarFallbackUtilsRequest& req);

// Get the current operational status
AvatarFallbackUtilsStatus avatar_fallback_utils_getStatus();

// Reset the module to initial state
void avatar_fallback_utils_reset();

// Validate input before processing
bool avatar_fallback_utils_validate(const std::string& input);

// Process input and return result
std::string avatar_fallback_utils_process(const std::string& input);

// Serialize to JSON format
json avatar_fallback_utils_toJson(const std::string& input);

// Get runtime statistics
AvatarFallbackUtilsStats avatar_fallback_utils_getStats();

// Build config from JSON
AvatarFallbackUtilsConfig avatar_fallback_utils_configFromJson(const json& j);

// Serialize config to JSON
json avatar_fallback_utils_configToJson(const AvatarFallbackUtilsConfig& config);
