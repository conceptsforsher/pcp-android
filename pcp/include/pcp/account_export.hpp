#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AccountExportState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* account_export_stateToString(AccountExportState s);
AccountExportState account_export_stateFromString(const std::string& s);

struct AccountExportConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AccountExportRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AccountExportResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AccountExportStatus {
    AccountExportState state = AccountExportState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AccountExportStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the account export module with configuration
bool account_export_init(const AccountExportConfig& config);

// Handle a account export request, returning a response
AccountExportResponse account_export_handle(const AccountExportRequest& req);

// Get the current operational status
AccountExportStatus account_export_getStatus();

// Reset the module to initial state
void account_export_reset();

// Validate input before processing
bool account_export_validate(const std::string& input);

// Process input and return result
std::string account_export_process(const std::string& input);

// Serialize to JSON format
json account_export_toJson(const std::string& input);

// Get runtime statistics
AccountExportStats account_export_getStats();

// Build config from JSON
AccountExportConfig account_export_configFromJson(const json& j);

// Serialize config to JSON
json account_export_configToJson(const AccountExportConfig& config);

} // namespace pcp
