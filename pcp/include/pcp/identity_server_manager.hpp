#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ServerManagerState {
    UNKNOWN = 0,
    UNVERIFIED = 1,
    PENDING = 2,
    VERIFIED = 3,
    REVOKED = 4
};

const char* identity_server_manager_stateToString(ServerManagerState s);
ServerManagerState identity_server_manager_stateFromString(const std::string& s);

struct IdentityServerManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct IdentityServerManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct IdentityServerManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct IdentityServerManagerStatus {
    ServerManagerState state = ServerManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct IdentityServerManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the identity server manager module with configuration
bool identity_server_manager_init(const IdentityServerManagerConfig& config);

// Handle a identity server manager request, returning a response
IdentityServerManagerResponse identity_server_manager_handle(const IdentityServerManagerRequest& req);

// Get the current operational status
IdentityServerManagerStatus identity_server_manager_getStatus();

// Reset the module to initial state
void identity_server_manager_reset();

// Validate input before processing
bool identity_server_manager_validate(const std::string& input);

// Process input and return result
std::string identity_server_manager_process(const std::string& input);

// Serialize to JSON format
json identity_server_manager_toJson(const std::string& input);

// Get runtime statistics
IdentityServerManagerStats identity_server_manager_getStats();

// Build config from JSON
IdentityServerManagerConfig identity_server_manager_configFromJson(const json& j);

// Serialize config to JSON
json identity_server_manager_configToJson(const IdentityServerManagerConfig& config);

} // namespace pcp
