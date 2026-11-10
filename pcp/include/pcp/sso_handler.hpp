#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SsoHandlerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* sso_handler_stateToString(SsoHandlerState s);
SsoHandlerState sso_handler_stateFromString(const std::string& s);

struct SsoHandlerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SsoHandlerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SsoHandlerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SsoHandlerStatus {
    SsoHandlerState state = SsoHandlerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SsoHandlerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sso handler module with configuration
bool sso_handler_init(const SsoHandlerConfig& config);

// Handle a sso handler request, returning a response
SsoHandlerResponse sso_handler_handle(const SsoHandlerRequest& req);

// Get the current operational status
SsoHandlerStatus sso_handler_getStatus();

// Reset the module to initial state
void sso_handler_reset();

// Validate input before processing
bool sso_handler_validate(const std::string& input);

// Process input and return result
std::string sso_handler_process(const std::string& input);

// Serialize to JSON format
json sso_handler_toJson(const std::string& input);

// Get runtime statistics
SsoHandlerStats sso_handler_getStats();

// Build config from JSON
SsoHandlerConfig sso_handler_configFromJson(const json& j);

// Serialize config to JSON
json sso_handler_configToJson(const SsoHandlerConfig& config);

} // namespace pcp
