#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UploadManagerType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_upload_manager_stateToString(UploadManagerType s);
UploadManagerType media_upload_manager_stateFromString(const std::string& s);

struct MediaUploadManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaUploadManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaUploadManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaUploadManagerStatus {
    UploadManagerType state = UploadManagerType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaUploadManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media upload manager module with configuration
bool media_upload_manager_init(const MediaUploadManagerConfig& config);

// Handle a media upload manager request, returning a response
MediaUploadManagerResponse media_upload_manager_handle(const MediaUploadManagerRequest& req);

// Get the current operational status
MediaUploadManagerStatus media_upload_manager_getStatus();

// Reset the module to initial state
void media_upload_manager_reset();

// Validate input before processing
bool media_upload_manager_validate(const std::string& input);

// Process input and return result
std::string media_upload_manager_process(const std::string& input);

// Serialize to JSON format
json media_upload_manager_toJson(const std::string& input);

// Get runtime statistics
MediaUploadManagerStats media_upload_manager_getStats();

// Build config from JSON
MediaUploadManagerConfig media_upload_manager_configFromJson(const json& j);

// Serialize config to JSON
json media_upload_manager_configToJson(const MediaUploadManagerConfig& config);

} // namespace pcp
