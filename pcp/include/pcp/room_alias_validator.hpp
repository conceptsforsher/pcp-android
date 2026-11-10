#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AliasValidatorAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_alias_validator_stateToString(AliasValidatorAction s);
AliasValidatorAction room_alias_validator_stateFromString(const std::string& s);

struct RoomAliasValidatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomAliasValidatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomAliasValidatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomAliasValidatorStatus {
    AliasValidatorAction state = AliasValidatorAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomAliasValidatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room alias validator module with configuration
bool room_alias_validator_init(const RoomAliasValidatorConfig& config);

// Handle a room alias validator request, returning a response
RoomAliasValidatorResponse room_alias_validator_handle(const RoomAliasValidatorRequest& req);

// Get the current operational status
RoomAliasValidatorStatus room_alias_validator_getStatus();

// Reset the module to initial state
void room_alias_validator_reset();

// Validate input before processing
bool room_alias_validator_validate(const std::string& input);

// Process input and return result
std::string room_alias_validator_process(const std::string& input);

// Serialize to JSON format
json room_alias_validator_toJson(const std::string& input);

// Get runtime statistics
RoomAliasValidatorStats room_alias_validator_getStats();

// Build config from JSON
RoomAliasValidatorConfig room_alias_validator_configFromJson(const json& j);

// Serialize config to JSON
json room_alias_validator_configToJson(const RoomAliasValidatorConfig& config);

} // namespace pcp
