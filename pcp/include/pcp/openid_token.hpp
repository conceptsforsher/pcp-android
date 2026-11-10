#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class OpenidTokenState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* openid_token_stateToString(OpenidTokenState s);
OpenidTokenState openid_token_stateFromString(const std::string& s);

struct OpenidTokenConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct OpenidTokenRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct OpenidTokenResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct OpenidTokenStatus {
    OpenidTokenState state = OpenidTokenState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct OpenidTokenStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the openid token module with configuration
bool openid_token_init(const OpenidTokenConfig& config);

// Handle a openid token request, returning a response
OpenidTokenResponse openid_token_handle(const OpenidTokenRequest& req);

// Get the current operational status
OpenidTokenStatus openid_token_getStatus();

// Reset the module to initial state
void openid_token_reset();

// Validate input before processing
bool openid_token_validate(const std::string& input);

// Process input and return result
std::string openid_token_process(const std::string& input);

// Serialize to JSON format
json openid_token_toJson(const std::string& input);

// Get runtime statistics
OpenidTokenStats openid_token_getStats();

// Build config from JSON
OpenidTokenConfig openid_token_configFromJson(const json& j);

// Serialize config to JSON
json openid_token_configToJson(const OpenidTokenConfig& config);

} // namespace pcp
