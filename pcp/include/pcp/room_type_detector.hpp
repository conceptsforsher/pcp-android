#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TypeDetectorAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_type_detector_stateToString(TypeDetectorAction s);
TypeDetectorAction room_type_detector_stateFromString(const std::string& s);

struct RoomTypeDetectorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomTypeDetectorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomTypeDetectorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomTypeDetectorStatus {
    TypeDetectorAction state = TypeDetectorAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomTypeDetectorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room type detector module with configuration
bool room_type_detector_init(const RoomTypeDetectorConfig& config);

// Handle a room type detector request, returning a response
RoomTypeDetectorResponse room_type_detector_handle(const RoomTypeDetectorRequest& req);

// Get the current operational status
RoomTypeDetectorStatus room_type_detector_getStatus();

// Reset the module to initial state
void room_type_detector_reset();

// Validate input before processing
bool room_type_detector_validate(const std::string& input);

// Process input and return result
std::string room_type_detector_process(const std::string& input);

// Serialize to JSON format
json room_type_detector_toJson(const std::string& input);

// Get runtime statistics
RoomTypeDetectorStats room_type_detector_getStats();

// Build config from JSON
RoomTypeDetectorConfig room_type_detector_configFromJson(const json& j);

// Serialize config to JSON
json room_type_detector_configToJson(const RoomTypeDetectorConfig& config);

} // namespace pcp
