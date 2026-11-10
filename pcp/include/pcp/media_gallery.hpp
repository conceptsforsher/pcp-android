#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class GalleryType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_gallery_stateToString(GalleryType s);
GalleryType media_gallery_stateFromString(const std::string& s);

struct MediaGalleryConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaGalleryRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaGalleryResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaGalleryStatus {
    GalleryType state = GalleryType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaGalleryStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media gallery module with configuration
bool media_gallery_init(const MediaGalleryConfig& config);

// Handle a media gallery request, returning a response
MediaGalleryResponse media_gallery_handle(const MediaGalleryRequest& req);

// Get the current operational status
MediaGalleryStatus media_gallery_getStatus();

// Reset the module to initial state
void media_gallery_reset();

// Validate input before processing
bool media_gallery_validate(const std::string& input);

// Process input and return result
std::string media_gallery_process(const std::string& input);

// Serialize to JSON format
json media_gallery_toJson(const std::string& input);

// Get runtime statistics
MediaGalleryStats media_gallery_getStats();

// Build config from JSON
MediaGalleryConfig media_gallery_configFromJson(const json& j);

// Serialize config to JSON
json media_gallery_configToJson(const MediaGalleryConfig& config);

} // namespace pcp
