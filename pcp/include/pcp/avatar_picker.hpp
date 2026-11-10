#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AvatarPickerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* avatar_picker_stateToString(AvatarPickerState s);
AvatarPickerState avatar_picker_stateFromString(const std::string& s);

struct AvatarPickerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AvatarPickerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AvatarPickerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AvatarPickerStatus {
    AvatarPickerState state = AvatarPickerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AvatarPickerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the avatar picker module with configuration
bool avatar_picker_init(const AvatarPickerConfig& config);

// Handle a avatar picker request, returning a response
AvatarPickerResponse avatar_picker_handle(const AvatarPickerRequest& req);

// Get the current operational status
AvatarPickerStatus avatar_picker_getStatus();

// Reset the module to initial state
void avatar_picker_reset();

// Validate input before processing
bool avatar_picker_validate(const std::string& input);

// Process input and return result
std::string avatar_picker_process(const std::string& input);

// Serialize to JSON format
json avatar_picker_toJson(const std::string& input);

// Get runtime statistics
AvatarPickerStats avatar_picker_getStats();

// Build config from JSON
AvatarPickerConfig avatar_picker_configFromJson(const json& j);

// Serialize config to JSON
json avatar_picker_configToJson(const AvatarPickerConfig& config);

} // namespace pcp
