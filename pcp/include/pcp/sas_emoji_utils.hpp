#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class SasEmojiUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* sas_emoji_utils_stateToString(SasEmojiUtilsState s);
SasEmojiUtilsState sas_emoji_utils_stateFromString(const std::string& s);

struct SasEmojiUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SasEmojiUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SasEmojiUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SasEmojiUtilsStatus {
    SasEmojiUtilsState state = SasEmojiUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SasEmojiUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sas emoji utils module with configuration
bool sas_emoji_utils_init(const SasEmojiUtilsConfig& config);

// Handle a sas emoji utils request, returning a response
SasEmojiUtilsResponse sas_emoji_utils_handle(const SasEmojiUtilsRequest& req);

// Get the current operational status
SasEmojiUtilsStatus sas_emoji_utils_getStatus();

// Reset the module to initial state
void sas_emoji_utils_reset();

// Validate input before processing
bool sas_emoji_utils_validate(const std::string& input);

// Process input and return result
std::string sas_emoji_utils_process(const std::string& input);

// Serialize to JSON format
json sas_emoji_utils_toJson(const std::string& input);

// Get runtime statistics
SasEmojiUtilsStats sas_emoji_utils_getStats();

// Build config from JSON
SasEmojiUtilsConfig sas_emoji_utils_configFromJson(const json& j);

// Serialize config to JSON
json sas_emoji_utils_configToJson(const SasEmojiUtilsConfig& config);
