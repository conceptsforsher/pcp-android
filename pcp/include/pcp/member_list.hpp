#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MemberListState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* member_list_stateToString(MemberListState s);
MemberListState member_list_stateFromString(const std::string& s);

struct MemberListConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MemberListRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MemberListResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MemberListStatus {
    MemberListState state = MemberListState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MemberListStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the member list module with configuration
bool member_list_init(const MemberListConfig& config);

// Handle a member list request, returning a response
MemberListResponse member_list_handle(const MemberListRequest& req);

// Get the current operational status
MemberListStatus member_list_getStatus();

// Reset the module to initial state
void member_list_reset();

// Validate input before processing
bool member_list_validate(const std::string& input);

// Process input and return result
std::string member_list_process(const std::string& input);

// Serialize to JSON format
json member_list_toJson(const std::string& input);

// Get runtime statistics
MemberListStats member_list_getStats();

// Build config from JSON
MemberListConfig member_list_configFromJson(const json& j);

// Serialize config to JSON
json member_list_configToJson(const MemberListConfig& config);

} // namespace pcp
