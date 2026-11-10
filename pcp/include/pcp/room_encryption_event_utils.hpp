#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EncryptionEventUtilsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_encryption_event_utils_stateToString(EncryptionEventUtilsAction s);
EncryptionEventUtilsAction room_encryption_event_utils_stateFromString(const std::string& s);

struct RoomEncryptionEventUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomEncryptionEventUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomEncryptionEventUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomEncryptionEventUtilsStatus {
    EncryptionEventUtilsAction state = EncryptionEventUtilsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomEncryptionEventUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room encryption event utils module with configuration
bool room_encryption_event_utils_init(const RoomEncryptionEventUtilsConfig& config);

// Handle a room encryption event utils request, returning a response
RoomEncryptionEventUtilsResponse room_encryption_event_utils_handle(const RoomEncryptionEventUtilsRequest& req);

// Get the current operational status
RoomEncryptionEventUtilsStatus room_encryption_event_utils_getStatus();

// Reset the module to initial state
void room_encryption_event_utils_reset();

// Validate input before processing
bool room_encryption_event_utils_validate(const std::string& input);

// Process input and return result
std::string room_encryption_event_utils_process(const std::string& input);

// Serialize to JSON format
json room_encryption_event_utils_toJson(const std::string& input);

// Get runtime statistics
RoomEncryptionEventUtilsStats room_encryption_event_utils_getStats();

// Build config from JSON
RoomEncryptionEventUtilsConfig room_encryption_event_utils_configFromJson(const json& j);

// Serialize config to JSON
json room_encryption_event_utils_configToJson(const RoomEncryptionEventUtilsConfig& config);

} // namespace pcp
