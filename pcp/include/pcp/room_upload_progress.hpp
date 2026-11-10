#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class UploadProgressAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_upload_progress_stateToString(UploadProgressAction s);
UploadProgressAction room_upload_progress_stateFromString(const std::string& s);

struct RoomUploadProgressConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomUploadProgressRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomUploadProgressResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomUploadProgressStatus {
    UploadProgressAction state = UploadProgressAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomUploadProgressStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room upload progress module with configuration
bool room_upload_progress_init(const RoomUploadProgressConfig& config);

// Handle a room upload progress request, returning a response
RoomUploadProgressResponse room_upload_progress_handle(const RoomUploadProgressRequest& req);

// Get the current operational status
RoomUploadProgressStatus room_upload_progress_getStatus();

// Reset the module to initial state
void room_upload_progress_reset();

// Validate input before processing
bool room_upload_progress_validate(const std::string& input);

// Process input and return result
std::string room_upload_progress_process(const std::string& input);

// Serialize to JSON format
json room_upload_progress_toJson(const std::string& input);

// Get runtime statistics
RoomUploadProgressStats room_upload_progress_getStats();

// Build config from JSON
RoomUploadProgressConfig room_upload_progress_configFromJson(const json& j);

// Serialize config to JSON
json room_upload_progress_configToJson(const RoomUploadProgressConfig& config);
