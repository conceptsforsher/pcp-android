#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ScheduledEditState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* scheduled_edit_stateToString(ScheduledEditState s);
ScheduledEditState scheduled_edit_stateFromString(const std::string& s);

struct ScheduledEditConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ScheduledEditRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ScheduledEditResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ScheduledEditStatus {
    ScheduledEditState state = ScheduledEditState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ScheduledEditStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the scheduled edit module with configuration
bool scheduled_edit_init(const ScheduledEditConfig& config);

// Handle a scheduled edit request, returning a response
ScheduledEditResponse scheduled_edit_handle(const ScheduledEditRequest& req);

// Get the current operational status
ScheduledEditStatus scheduled_edit_getStatus();

// Reset the module to initial state
void scheduled_edit_reset();

// Validate input before processing
bool scheduled_edit_validate(const std::string& input);

// Process input and return result
std::string scheduled_edit_process(const std::string& input);

// Serialize to JSON format
json scheduled_edit_toJson(const std::string& input);

// Get runtime statistics
ScheduledEditStats scheduled_edit_getStats();

// Build config from JSON
ScheduledEditConfig scheduled_edit_configFromJson(const json& j);

// Serialize config to JSON
json scheduled_edit_configToJson(const ScheduledEditConfig& config);

} // namespace pcp
