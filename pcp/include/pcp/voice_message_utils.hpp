#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class VoiceMessageUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* voice_message_utils_stateToString(VoiceMessageUtilsState s);
VoiceMessageUtilsState voice_message_utils_stateFromString(const std::string& s);

struct VoiceMessageUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct VoiceMessageUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct VoiceMessageUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct VoiceMessageUtilsStatus {
    VoiceMessageUtilsState state = VoiceMessageUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct VoiceMessageUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the voice message utils module with configuration
bool voice_message_utils_init(const VoiceMessageUtilsConfig& config);

// Handle a voice message utils request, returning a response
VoiceMessageUtilsResponse voice_message_utils_handle(const VoiceMessageUtilsRequest& req);

// Get the current operational status
VoiceMessageUtilsStatus voice_message_utils_getStatus();

// Reset the module to initial state
void voice_message_utils_reset();

// Validate input before processing
bool voice_message_utils_validate(const std::string& input);

// Process input and return result
std::string voice_message_utils_process(const std::string& input);

// Serialize to JSON format
json voice_message_utils_toJson(const std::string& input);

// Get runtime statistics
VoiceMessageUtilsStats voice_message_utils_getStats();

// Build config from JSON
VoiceMessageUtilsConfig voice_message_utils_configFromJson(const json& j);

// Serialize config to JSON
json voice_message_utils_configToJson(const VoiceMessageUtilsConfig& config);

} // namespace pcp
