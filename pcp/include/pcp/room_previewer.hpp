#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PreviewerAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_previewer_stateToString(PreviewerAction s);
PreviewerAction room_previewer_stateFromString(const std::string& s);

struct RoomPreviewerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomPreviewerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomPreviewerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomPreviewerStatus {
    PreviewerAction state = PreviewerAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomPreviewerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room previewer module with configuration
bool room_previewer_init(const RoomPreviewerConfig& config);

// Handle a room previewer request, returning a response
RoomPreviewerResponse room_previewer_handle(const RoomPreviewerRequest& req);

// Get the current operational status
RoomPreviewerStatus room_previewer_getStatus();

// Reset the module to initial state
void room_previewer_reset();

// Validate input before processing
bool room_previewer_validate(const std::string& input);

// Process input and return result
std::string room_previewer_process(const std::string& input);

// Serialize to JSON format
json room_previewer_toJson(const std::string& input);

// Get runtime statistics
RoomPreviewerStats room_previewer_getStats();

// Build config from JSON
RoomPreviewerConfig room_previewer_configFromJson(const json& j);

// Serialize config to JSON
json room_previewer_configToJson(const RoomPreviewerConfig& config);

} // namespace pcp
