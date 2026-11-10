#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ConfigUtilsType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_config_utils_stateToString(ConfigUtilsType s);
ConfigUtilsType media_config_utils_stateFromString(const std::string& s);

struct MediaConfigUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaConfigUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaConfigUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaConfigUtilsStatus {
    ConfigUtilsType state = ConfigUtilsType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaConfigUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media config utils module with configuration
bool media_config_utils_init(const MediaConfigUtilsConfig& config);

// Handle a media config utils request, returning a response
MediaConfigUtilsResponse media_config_utils_handle(const MediaConfigUtilsRequest& req);

// Get the current operational status
MediaConfigUtilsStatus media_config_utils_getStatus();

// Reset the module to initial state
void media_config_utils_reset();

// Validate input before processing
bool media_config_utils_validate(const std::string& input);

// Process input and return result
std::string media_config_utils_process(const std::string& input);

// Serialize to JSON format
json media_config_utils_toJson(const std::string& input);

// Get runtime statistics
MediaConfigUtilsStats media_config_utils_getStats();

// Build config from JSON
MediaConfigUtilsConfig media_config_utils_configFromJson(const json& j);

// Serialize config to JSON
json media_config_utils_configToJson(const MediaConfigUtilsConfig& config);
