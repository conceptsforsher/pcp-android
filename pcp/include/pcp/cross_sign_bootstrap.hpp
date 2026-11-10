#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BootstrapPhase {
    UNSET = 0,
    PENDING = 1,
    BOOTSTRAPPING = 2,
    SIGNED = 3,
    VERIFIED = 4
};

const char* cross_sign_bootstrap_stateToString(BootstrapPhase s);
BootstrapPhase cross_sign_bootstrap_stateFromString(const std::string& s);

struct CrossSignBootstrapConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CrossSignBootstrapRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CrossSignBootstrapResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CrossSignBootstrapStatus {
    BootstrapPhase state = BootstrapPhase::UNSET;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CrossSignBootstrapStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the cross sign bootstrap module with configuration
bool cross_sign_bootstrap_init(const CrossSignBootstrapConfig& config);

// Handle a cross sign bootstrap request, returning a response
CrossSignBootstrapResponse cross_sign_bootstrap_handle(const CrossSignBootstrapRequest& req);

// Get the current operational status
CrossSignBootstrapStatus cross_sign_bootstrap_getStatus();

// Reset the module to initial state
void cross_sign_bootstrap_reset();

// Validate input before processing
bool cross_sign_bootstrap_validate(const std::string& input);

// Process input and return result
std::string cross_sign_bootstrap_process(const std::string& input);

// Serialize to JSON format
json cross_sign_bootstrap_toJson(const std::string& input);

// Get runtime statistics
CrossSignBootstrapStats cross_sign_bootstrap_getStats();

// Build config from JSON
CrossSignBootstrapConfig cross_sign_bootstrap_configFromJson(const json& j);

// Serialize config to JSON
json cross_sign_bootstrap_configToJson(const CrossSignBootstrapConfig& config);

} // namespace pcp
