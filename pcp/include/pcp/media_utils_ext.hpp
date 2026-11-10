#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UtilsExtType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_utils_ext_stateToString(UtilsExtType s);
UtilsExtType media_utils_ext_stateFromString(const std::string& s);

struct MediaUtilsExtConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaUtilsExtRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaUtilsExtResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaUtilsExtStatus {
    UtilsExtType state = UtilsExtType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaUtilsExtStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media utils ext module with configuration
bool media_utils_ext_init(const MediaUtilsExtConfig& config);

// Handle a media utils ext request, returning a response
MediaUtilsExtResponse media_utils_ext_handle(const MediaUtilsExtRequest& req);

// Get the current operational status
MediaUtilsExtStatus media_utils_ext_getStatus();

// Reset the module to initial state
void media_utils_ext_reset();

// Validate input before processing
bool media_utils_ext_validate(const std::string& input);

// Process input and return result
std::string media_utils_ext_process(const std::string& input);

// Serialize to JSON format
json media_utils_ext_toJson(const std::string& input);

// Get runtime statistics
MediaUtilsExtStats media_utils_ext_getStats();

// Build config from JSON
MediaUtilsExtConfig media_utils_ext_configFromJson(const json& j);

// Serialize config to JSON
json media_utils_ext_configToJson(const MediaUtilsExtConfig& config);

} // namespace pcp
