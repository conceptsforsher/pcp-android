#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ProgressUtilsType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_progress_utils_stateToString(ProgressUtilsType s);
ProgressUtilsType media_progress_utils_stateFromString(const std::string& s);

struct MediaProgressUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaProgressUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaProgressUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaProgressUtilsStatus {
    ProgressUtilsType state = ProgressUtilsType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaProgressUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media progress utils module with configuration
bool media_progress_utils_init(const MediaProgressUtilsConfig& config);

// Handle a media progress utils request, returning a response
MediaProgressUtilsResponse media_progress_utils_handle(const MediaProgressUtilsRequest& req);

// Get the current operational status
MediaProgressUtilsStatus media_progress_utils_getStatus();

// Reset the module to initial state
void media_progress_utils_reset();

// Validate input before processing
bool media_progress_utils_validate(const std::string& input);

// Process input and return result
std::string media_progress_utils_process(const std::string& input);

// Serialize to JSON format
json media_progress_utils_toJson(const std::string& input);

// Get runtime statistics
MediaProgressUtilsStats media_progress_utils_getStats();

// Build config from JSON
MediaProgressUtilsConfig media_progress_utils_configFromJson(const json& j);

// Serialize config to JSON
json media_progress_utils_configToJson(const MediaProgressUtilsConfig& config);

} // namespace pcp
