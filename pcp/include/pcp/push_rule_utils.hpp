#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class RuleUtilsKind {
    UNKNOWN = 0,
    MESSAGE = 1,
    INVITE = 2,
    CALL = 3,
    NOTIFICATION = 4
};

const char* push_rule_utils_stateToString(RuleUtilsKind s);
RuleUtilsKind push_rule_utils_stateFromString(const std::string& s);

struct PushRuleUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PushRuleUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PushRuleUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PushRuleUtilsStatus {
    RuleUtilsKind state = RuleUtilsKind::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PushRuleUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the push rule utils module with configuration
bool push_rule_utils_init(const PushRuleUtilsConfig& config);

// Handle a push rule utils request, returning a response
PushRuleUtilsResponse push_rule_utils_handle(const PushRuleUtilsRequest& req);

// Get the current operational status
PushRuleUtilsStatus push_rule_utils_getStatus();

// Reset the module to initial state
void push_rule_utils_reset();

// Validate input before processing
bool push_rule_utils_validate(const std::string& input);

// Process input and return result
std::string push_rule_utils_process(const std::string& input);

// Serialize to JSON format
json push_rule_utils_toJson(const std::string& input);

// Get runtime statistics
PushRuleUtilsStats push_rule_utils_getStats();

// Build config from JSON
PushRuleUtilsConfig push_rule_utils_configFromJson(const json& j);

// Serialize config to JSON
json push_rule_utils_configToJson(const PushRuleUtilsConfig& config);

} // namespace pcp
