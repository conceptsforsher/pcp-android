#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CreateRoomState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* create_room_stateToString(CreateRoomState s);
CreateRoomState create_room_stateFromString(const std::string& s);

struct CreateRoomConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CreateRoomRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CreateRoomResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CreateRoomStatus {
    CreateRoomState state = CreateRoomState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CreateRoomStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the create room module with configuration
bool create_room_init(const CreateRoomConfig& config);

// Handle a create room request, returning a response
CreateRoomResponse create_room_handle(const CreateRoomRequest& req);

// Get the current operational status
CreateRoomStatus create_room_getStatus();

// Reset the module to initial state
void create_room_reset();

// Validate input before processing
bool create_room_validate(const std::string& input);

// Process input and return result
std::string create_room_process(const std::string& input);

// Serialize to JSON format
json create_room_toJson(const std::string& input);

// Get runtime statistics
CreateRoomStats create_room_getStats();

// Build config from JSON
CreateRoomConfig create_room_configFromJson(const json& j);

// Serialize config to JSON
json create_room_configToJson(const CreateRoomConfig& config);

} // namespace pcp
