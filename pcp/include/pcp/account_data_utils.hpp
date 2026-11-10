#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AccountDataUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* account_data_utils_stateToString(AccountDataUtilsState s);
AccountDataUtilsState account_data_utils_stateFromString(const std::string& s);

struct AccountDataUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AccountDataUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AccountDataUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AccountDataUtilsStatus {
    AccountDataUtilsState state = AccountDataUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AccountDataUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the account data utils module with configuration
bool account_data_utils_init(const AccountDataUtilsConfig& config);

// Handle a account data utils request, returning a response
AccountDataUtilsResponse account_data_utils_handle(const AccountDataUtilsRequest& req);

// Get the current operational status
AccountDataUtilsStatus account_data_utils_getStatus();

// Reset the module to initial state
void account_data_utils_reset();

// Validate input before processing
bool account_data_utils_validate(const std::string& input);

// Process input and return result
std::string account_data_utils_process(const std::string& input);

// Serialize to JSON format
json account_data_utils_toJson(const std::string& input);

// Get runtime statistics
AccountDataUtilsStats account_data_utils_getStats();

// Build config from JSON
AccountDataUtilsConfig account_data_utils_configFromJson(const json& j);

// Serialize config to JSON
json account_data_utils_configToJson(const AccountDataUtilsConfig& config);

} // namespace pcp
