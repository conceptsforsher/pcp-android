#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReportUtilsState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_report_utils_stateToString(ReportUtilsState s);
ReportUtilsState user_report_utils_stateFromString(const std::string& s);

struct UserReportUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserReportUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserReportUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserReportUtilsStatus {
    ReportUtilsState state = ReportUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserReportUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user report utils module with configuration
bool user_report_utils_init(const UserReportUtilsConfig& config);

// Handle a user report utils request, returning a response
UserReportUtilsResponse user_report_utils_handle(const UserReportUtilsRequest& req);

// Get the current operational status
UserReportUtilsStatus user_report_utils_getStatus();

// Reset the module to initial state
void user_report_utils_reset();

// Validate input before processing
bool user_report_utils_validate(const std::string& input);

// Process input and return result
std::string user_report_utils_process(const std::string& input);

// Serialize to JSON format
json user_report_utils_toJson(const std::string& input);

// Get runtime statistics
UserReportUtilsStats user_report_utils_getStats();

// Build config from JSON
UserReportUtilsConfig user_report_utils_configFromJson(const json& j);

// Serialize config to JSON
json user_report_utils_configToJson(const UserReportUtilsConfig& config);

} // namespace pcp
