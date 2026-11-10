#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AccountValidatorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* account_validator_stateToString(AccountValidatorState s);
AccountValidatorState account_validator_stateFromString(const std::string& s);

struct AccountValidatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AccountValidatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AccountValidatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AccountValidatorStatus {
    AccountValidatorState state = AccountValidatorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AccountValidatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the account validator module with configuration
bool account_validator_init(const AccountValidatorConfig& config);

// Handle a account validator request, returning a response
AccountValidatorResponse account_validator_handle(const AccountValidatorRequest& req);

// Get the current operational status
AccountValidatorStatus account_validator_getStatus();

// Reset the module to initial state
void account_validator_reset();

// Validate input before processing
bool account_validator_validate(const std::string& input);

// Process input and return result
std::string account_validator_process(const std::string& input);

// Serialize to JSON format
json account_validator_toJson(const std::string& input);

// Get runtime statistics
AccountValidatorStats account_validator_getStats();

// Build config from JSON
AccountValidatorConfig account_validator_configFromJson(const json& j);

// Serialize config to JSON
json account_validator_configToJson(const AccountValidatorConfig& config);

} // namespace pcp
