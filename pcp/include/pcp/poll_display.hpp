#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PollDisplayState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* poll_display_stateToString(PollDisplayState s);
PollDisplayState poll_display_stateFromString(const std::string& s);

struct PollDisplayConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PollDisplayRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PollDisplayResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PollDisplayStatus {
    PollDisplayState state = PollDisplayState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PollDisplayStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the poll display module with configuration
bool poll_display_init(const PollDisplayConfig& config);

// Handle a poll display request, returning a response
PollDisplayResponse poll_display_handle(const PollDisplayRequest& req);

// Get the current operational status
PollDisplayStatus poll_display_getStatus();

// Reset the module to initial state
void poll_display_reset();

// Validate input before processing
bool poll_display_validate(const std::string& input);

// Process input and return result
std::string poll_display_process(const std::string& input);

// Serialize to JSON format
json poll_display_toJson(const std::string& input);

// Get runtime statistics
PollDisplayStats poll_display_getStats();

// Build config from JSON
PollDisplayConfig poll_display_configFromJson(const json& j);

// Serialize config to JSON
json poll_display_configToJson(const PollDisplayConfig& config);

} // namespace pcp
