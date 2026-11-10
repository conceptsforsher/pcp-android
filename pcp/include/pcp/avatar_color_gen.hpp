#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AvatarColorGenState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* avatar_color_gen_stateToString(AvatarColorGenState s);
AvatarColorGenState avatar_color_gen_stateFromString(const std::string& s);

struct AvatarColorGenConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AvatarColorGenRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AvatarColorGenResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AvatarColorGenStatus {
    AvatarColorGenState state = AvatarColorGenState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AvatarColorGenStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the avatar color gen module with configuration
bool avatar_color_gen_init(const AvatarColorGenConfig& config);

// Handle a avatar color gen request, returning a response
AvatarColorGenResponse avatar_color_gen_handle(const AvatarColorGenRequest& req);

// Get the current operational status
AvatarColorGenStatus avatar_color_gen_getStatus();

// Reset the module to initial state
void avatar_color_gen_reset();

// Validate input before processing
bool avatar_color_gen_validate(const std::string& input);

// Process input and return result
std::string avatar_color_gen_process(const std::string& input);

// Serialize to JSON format
json avatar_color_gen_toJson(const std::string& input);

// Get runtime statistics
AvatarColorGenStats avatar_color_gen_getStats();

// Build config from JSON
AvatarColorGenConfig avatar_color_gen_configFromJson(const json& j);

// Serialize config to JSON
json avatar_color_gen_configToJson(const AvatarColorGenConfig& config);

} // namespace pcp
