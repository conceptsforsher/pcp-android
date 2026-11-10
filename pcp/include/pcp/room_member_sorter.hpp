#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MemberSorterAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_member_sorter_stateToString(MemberSorterAction s);
MemberSorterAction room_member_sorter_stateFromString(const std::string& s);

struct RoomMemberSorterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomMemberSorterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomMemberSorterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomMemberSorterStatus {
    MemberSorterAction state = MemberSorterAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomMemberSorterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room member sorter module with configuration
bool room_member_sorter_init(const RoomMemberSorterConfig& config);

// Handle a room member sorter request, returning a response
RoomMemberSorterResponse room_member_sorter_handle(const RoomMemberSorterRequest& req);

// Get the current operational status
RoomMemberSorterStatus room_member_sorter_getStatus();

// Reset the module to initial state
void room_member_sorter_reset();

// Validate input before processing
bool room_member_sorter_validate(const std::string& input);

// Process input and return result
std::string room_member_sorter_process(const std::string& input);

// Serialize to JSON format
json room_member_sorter_toJson(const std::string& input);

// Get runtime statistics
RoomMemberSorterStats room_member_sorter_getStats();

// Build config from JSON
RoomMemberSorterConfig room_member_sorter_configFromJson(const json& j);

// Serialize config to JSON
json room_member_sorter_configToJson(const RoomMemberSorterConfig& config);

} // namespace pcp
