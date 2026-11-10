#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class KickerAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_kicker_stateToString(KickerAction s);
KickerAction room_kicker_stateFromString(const std::string& s);

struct RoomKickerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomKickerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomKickerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomKickerStatus {
    KickerAction state = KickerAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomKickerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room kicker module with configuration
bool room_kicker_init(const RoomKickerConfig& config);

// Handle a room kicker request, returning a response
RoomKickerResponse room_kicker_handle(const RoomKickerRequest& req);

// Get the current operational status
RoomKickerStatus room_kicker_getStatus();

// Reset the module to initial state
void room_kicker_reset();

// Validate input before processing
bool room_kicker_validate(const std::string& input);

// Process input and return result
std::string room_kicker_process(const std::string& input);

// Serialize to JSON format
json room_kicker_toJson(const std::string& input);

// Get runtime statistics
RoomKickerStats room_kicker_getStats();

// Build config from JSON
RoomKickerConfig room_kicker_configFromJson(const json& j);

// Serialize config to JSON
json room_kicker_configToJson(const RoomKickerConfig& config);

} // namespace pcp
