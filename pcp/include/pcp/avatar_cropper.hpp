#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AvatarCropperState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* avatar_cropper_stateToString(AvatarCropperState s);
AvatarCropperState avatar_cropper_stateFromString(const std::string& s);

struct AvatarCropperConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AvatarCropperRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AvatarCropperResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AvatarCropperStatus {
    AvatarCropperState state = AvatarCropperState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AvatarCropperStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the avatar cropper module with configuration
bool avatar_cropper_init(const AvatarCropperConfig& config);

// Handle a avatar cropper request, returning a response
AvatarCropperResponse avatar_cropper_handle(const AvatarCropperRequest& req);

// Get the current operational status
AvatarCropperStatus avatar_cropper_getStatus();

// Reset the module to initial state
void avatar_cropper_reset();

// Validate input before processing
bool avatar_cropper_validate(const std::string& input);

// Process input and return result
std::string avatar_cropper_process(const std::string& input);

// Serialize to JSON format
json avatar_cropper_toJson(const std::string& input);

// Get runtime statistics
AvatarCropperStats avatar_cropper_getStats();

// Build config from JSON
AvatarCropperConfig avatar_cropper_configFromJson(const json& j);

// Serialize config to JSON
json avatar_cropper_configToJson(const AvatarCropperConfig& config);

} // namespace pcp
