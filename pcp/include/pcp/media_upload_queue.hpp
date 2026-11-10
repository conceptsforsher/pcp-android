#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UploadQueueType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_upload_queue_stateToString(UploadQueueType s);
UploadQueueType media_upload_queue_stateFromString(const std::string& s);

struct MediaUploadQueueConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaUploadQueueRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaUploadQueueResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaUploadQueueStatus {
    UploadQueueType state = UploadQueueType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaUploadQueueStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media upload queue module with configuration
bool media_upload_queue_init(const MediaUploadQueueConfig& config);

// Handle a media upload queue request, returning a response
MediaUploadQueueResponse media_upload_queue_handle(const MediaUploadQueueRequest& req);

// Get the current operational status
MediaUploadQueueStatus media_upload_queue_getStatus();

// Reset the module to initial state
void media_upload_queue_reset();

// Validate input before processing
bool media_upload_queue_validate(const std::string& input);

// Process input and return result
std::string media_upload_queue_process(const std::string& input);

// Serialize to JSON format
json media_upload_queue_toJson(const std::string& input);

// Get runtime statistics
MediaUploadQueueStats media_upload_queue_getStats();

// Build config from JSON
MediaUploadQueueConfig media_upload_queue_configFromJson(const json& j);

// Serialize config to JSON
json media_upload_queue_configToJson(const MediaUploadQueueConfig& config);

} // namespace pcp
