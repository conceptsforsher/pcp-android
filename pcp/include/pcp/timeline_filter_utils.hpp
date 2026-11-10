#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TimelineFilterUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* timeline_filter_utils_stateToString(TimelineFilterUtilsState s);
TimelineFilterUtilsState timeline_filter_utils_stateFromString(const std::string& s);

struct TimelineFilterUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct TimelineFilterUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct TimelineFilterUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct TimelineFilterUtilsStatus {
    TimelineFilterUtilsState state = TimelineFilterUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct TimelineFilterUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the timeline filter utils module with configuration
bool timeline_filter_utils_init(const TimelineFilterUtilsConfig& config);

// Handle a timeline filter utils request, returning a response
TimelineFilterUtilsResponse timeline_filter_utils_handle(const TimelineFilterUtilsRequest& req);

// Get the current operational status
TimelineFilterUtilsStatus timeline_filter_utils_getStatus();

// Reset the module to initial state
void timeline_filter_utils_reset();

// Validate input before processing
bool timeline_filter_utils_validate(const std::string& input);

// Process input and return result
std::string timeline_filter_utils_process(const std::string& input);

// Serialize to JSON format
json timeline_filter_utils_toJson(const std::string& input);

// Get runtime statistics
TimelineFilterUtilsStats timeline_filter_utils_getStats();

// Build config from JSON
TimelineFilterUtilsConfig timeline_filter_utils_configFromJson(const json& j);

// Serialize config to JSON
json timeline_filter_utils_configToJson(const TimelineFilterUtilsConfig& config);

} // namespace pcp
