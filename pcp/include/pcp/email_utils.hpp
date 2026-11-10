#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EmailUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* email_utils_stateToString(EmailUtilsState s);
EmailUtilsState email_utils_stateFromString(const std::string& s);

struct EmailUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EmailUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EmailUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EmailUtilsStatus {
    EmailUtilsState state = EmailUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EmailUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the email utils module with configuration
bool email_utils_init(const EmailUtilsConfig& config);

// Handle a email utils request, returning a response
EmailUtilsResponse email_utils_handle(const EmailUtilsRequest& req);

// Get the current operational status
EmailUtilsStatus email_utils_getStatus();

// Reset the module to initial state
void email_utils_reset();

// Validate input before processing
bool email_utils_validate(const std::string& input);

// Process input and return result
std::string email_utils_process(const std::string& input);

// Serialize to JSON format
json email_utils_toJson(const std::string& input);

// Get runtime statistics
EmailUtilsStats email_utils_getStats();

// Build config from JSON
EmailUtilsConfig email_utils_configFromJson(const json& j);

// Serialize config to JSON
json email_utils_configToJson(const EmailUtilsConfig& config);

} // namespace pcp
