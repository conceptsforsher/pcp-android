#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PublicRoomListerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* public_room_lister_stateToString(PublicRoomListerState s);
PublicRoomListerState public_room_lister_stateFromString(const std::string& s);

struct PublicRoomListerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PublicRoomListerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PublicRoomListerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PublicRoomListerStatus {
    PublicRoomListerState state = PublicRoomListerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PublicRoomListerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the public room lister module with configuration
bool public_room_lister_init(const PublicRoomListerConfig& config);

// Handle a public room lister request, returning a response
PublicRoomListerResponse public_room_lister_handle(const PublicRoomListerRequest& req);

// Get the current operational status
PublicRoomListerStatus public_room_lister_getStatus();

// Reset the module to initial state
void public_room_lister_reset();

// Validate input before processing
bool public_room_lister_validate(const std::string& input);

// Process input and return result
std::string public_room_lister_process(const std::string& input);

// Serialize to JSON format
json public_room_lister_toJson(const std::string& input);

// Get runtime statistics
PublicRoomListerStats public_room_lister_getStats();

// Build config from JSON
PublicRoomListerConfig public_room_lister_configFromJson(const json& j);

// Serialize config to JSON
json public_room_lister_configToJson(const PublicRoomListerConfig& config);

} // namespace pcp
