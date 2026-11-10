#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class MentionUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* mention_utils_stateToString(MentionUtilsState s);
MentionUtilsState mention_utils_stateFromString(const std::string& s);

struct MentionUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MentionUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MentionUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MentionUtilsStatus {
    MentionUtilsState state = MentionUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MentionUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the mention utils module with configuration
bool mention_utils_init(const MentionUtilsConfig& config);

// Handle a mention utils request, returning a response
MentionUtilsResponse mention_utils_handle(const MentionUtilsRequest& req);

// Get the current operational status
MentionUtilsStatus mention_utils_getStatus();

// Reset the module to initial state
void mention_utils_reset();

// Validate input before processing
bool mention_utils_validate(const std::string& input);

// Process input and return result
std::string mention_utils_process(const std::string& input);

// Serialize to JSON format
json mention_utils_toJson(const std::string& input);

// Get runtime statistics
MentionUtilsStats mention_utils_getStats();

// Build config from JSON
MentionUtilsConfig mention_utils_configFromJson(const json& j);

// Serialize config to JSON
json mention_utils_configToJson(const MentionUtilsConfig& config);
