#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SuggestionsAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_suggestions_stateToString(SuggestionsAction s);
SuggestionsAction room_suggestions_stateFromString(const std::string& s);

struct RoomSuggestionsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomSuggestionsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomSuggestionsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomSuggestionsStatus {
    SuggestionsAction state = SuggestionsAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomSuggestionsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room suggestions module with configuration
bool room_suggestions_init(const RoomSuggestionsConfig& config);

// Handle a room suggestions request, returning a response
RoomSuggestionsResponse room_suggestions_handle(const RoomSuggestionsRequest& req);

// Get the current operational status
RoomSuggestionsStatus room_suggestions_getStatus();

// Reset the module to initial state
void room_suggestions_reset();

// Validate input before processing
bool room_suggestions_validate(const std::string& input);

// Process input and return result
std::string room_suggestions_process(const std::string& input);

// Serialize to JSON format
json room_suggestions_toJson(const std::string& input);

// Get runtime statistics
RoomSuggestionsStats room_suggestions_getStats();

// Build config from JSON
RoomSuggestionsConfig room_suggestions_configFromJson(const json& j);

// Serialize config to JSON
json room_suggestions_configToJson(const RoomSuggestionsConfig& config);

} // namespace pcp
