#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DownloadType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_download_stateToString(DownloadType s);
DownloadType media_download_stateFromString(const std::string& s);

struct MediaDownloadConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaDownloadRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaDownloadResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaDownloadStatus {
    DownloadType state = DownloadType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaDownloadStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media download module with configuration
bool media_download_init(const MediaDownloadConfig& config);

// Handle a media download request, returning a response
MediaDownloadResponse media_download_handle(const MediaDownloadRequest& req);

// Get the current operational status
MediaDownloadStatus media_download_getStatus();

// Reset the module to initial state
void media_download_reset();

// Validate input before processing
bool media_download_validate(const std::string& input);

// Process input and return result
std::string media_download_process(const std::string& input);

// Serialize to JSON format
json media_download_toJson(const std::string& input);

// Get runtime statistics
MediaDownloadStats media_download_getStats();

// Build config from JSON
MediaDownloadConfig media_download_configFromJson(const json& j);

// Serialize config to JSON
json media_download_configToJson(const MediaDownloadConfig& config);

} // namespace pcp
