#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ServerLookupState {
    UNKNOWN = 0,
    UNVERIFIED = 1,
    PENDING = 2,
    VERIFIED = 3,
    REVOKED = 4
};

const char* identity_server_lookup_stateToString(ServerLookupState s);
ServerLookupState identity_server_lookup_stateFromString(const std::string& s);

struct IdentityServerLookupConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct IdentityServerLookupRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct IdentityServerLookupResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct IdentityServerLookupStatus {
    ServerLookupState state = ServerLookupState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct IdentityServerLookupStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the identity server lookup module with configuration
bool identity_server_lookup_init(const IdentityServerLookupConfig& config);

// Handle a identity server lookup request, returning a response
IdentityServerLookupResponse identity_server_lookup_handle(const IdentityServerLookupRequest& req);

// Get the current operational status
IdentityServerLookupStatus identity_server_lookup_getStatus();

// Reset the module to initial state
void identity_server_lookup_reset();

// Validate input before processing
bool identity_server_lookup_validate(const std::string& input);

// Process input and return result
std::string identity_server_lookup_process(const std::string& input);

// Serialize to JSON format
json identity_server_lookup_toJson(const std::string& input);

// Get runtime statistics
IdentityServerLookupStats identity_server_lookup_getStats();

// Build config from JSON
IdentityServerLookupConfig identity_server_lookup_configFromJson(const json& j);

// Serialize config to JSON
json identity_server_lookup_configToJson(const IdentityServerLookupConfig& config);

} // namespace pcp
