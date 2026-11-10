#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class SoundUtilsLevel {
    SILENT = 0,
    MENTION = 1,
    HIGHLIGHT = 2,
    ALL = 3,
    DEFAULT = 4
};

const char* notification_sound_utils_stateToString(SoundUtilsLevel s);
SoundUtilsLevel notification_sound_utils_stateFromString(const std::string& s);

struct NotificationSoundUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotificationSoundUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotificationSoundUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotificationSoundUtilsStatus {
    SoundUtilsLevel state = SoundUtilsLevel::SILENT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotificationSoundUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notification sound utils module with configuration
bool notification_sound_utils_init(const NotificationSoundUtilsConfig& config);

// Handle a notification sound utils request, returning a response
NotificationSoundUtilsResponse notification_sound_utils_handle(const NotificationSoundUtilsRequest& req);

// Get the current operational status
NotificationSoundUtilsStatus notification_sound_utils_getStatus();

// Reset the module to initial state
void notification_sound_utils_reset();

// Validate input before processing
bool notification_sound_utils_validate(const std::string& input);

// Process input and return result
std::string notification_sound_utils_process(const std::string& input);

// Serialize to JSON format
json notification_sound_utils_toJson(const std::string& input);

// Get runtime statistics
NotificationSoundUtilsStats notification_sound_utils_getStats();

// Build config from JSON
NotificationSoundUtilsConfig notification_sound_utils_configFromJson(const json& j);

// Serialize config to JSON
json notification_sound_utils_configToJson(const NotificationSoundUtilsConfig& config);
