#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReadMarkerUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* read_marker_utils_stateToString(ReadMarkerUtilsState s);
ReadMarkerUtilsState read_marker_utils_stateFromString(const std::string& s);

struct ReadMarkerUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReadMarkerUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReadMarkerUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReadMarkerUtilsStatus {
    ReadMarkerUtilsState state = ReadMarkerUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReadMarkerUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the read marker utils module with configuration
bool read_marker_utils_init(const ReadMarkerUtilsConfig& config);

// Handle a read marker utils request, returning a response
ReadMarkerUtilsResponse read_marker_utils_handle(const ReadMarkerUtilsRequest& req);

// Get the current operational status
ReadMarkerUtilsStatus read_marker_utils_getStatus();

// Reset the module to initial state
void read_marker_utils_reset();

// Validate input before processing
bool read_marker_utils_validate(const std::string& input);

// Process input and return result
std::string read_marker_utils_process(const std::string& input);

// Serialize to JSON format
json read_marker_utils_toJson(const std::string& input);

// Get runtime statistics
ReadMarkerUtilsStats read_marker_utils_getStats();

// Build config from JSON
ReadMarkerUtilsConfig read_marker_utils_configFromJson(const json& j);

// Serialize config to JSON
json read_marker_utils_configToJson(const ReadMarkerUtilsConfig& config);

} // namespace pcp
