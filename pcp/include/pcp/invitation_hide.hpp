#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class InvitationHideState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* invitation_hide_stateToString(InvitationHideState s);
InvitationHideState invitation_hide_stateFromString(const std::string& s);

struct InvitationHideConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct InvitationHideRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct InvitationHideResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct InvitationHideStatus {
    InvitationHideState state = InvitationHideState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct InvitationHideStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the invitation hide module with configuration
bool invitation_hide_init(const InvitationHideConfig& config);

// Handle a invitation hide request, returning a response
InvitationHideResponse invitation_hide_handle(const InvitationHideRequest& req);

// Get the current operational status
InvitationHideStatus invitation_hide_getStatus();

// Reset the module to initial state
void invitation_hide_reset();

// Validate input before processing
bool invitation_hide_validate(const std::string& input);

// Process input and return result
std::string invitation_hide_process(const std::string& input);

// Serialize to JSON format
json invitation_hide_toJson(const std::string& input);

// Get runtime statistics
InvitationHideStats invitation_hide_getStats();

// Build config from JSON
InvitationHideConfig invitation_hide_configFromJson(const json& j);

// Serialize config to JSON
json invitation_hide_configToJson(const InvitationHideConfig& config);

} // namespace pcp
