#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReportUtilsPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_report_utils_stateToString(ReportUtilsPhase s);
ReportUtilsPhase message_report_utils_stateFromString(const std::string& s);

struct MessageReportUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageReportUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageReportUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageReportUtilsStatus {
    ReportUtilsPhase state = ReportUtilsPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageReportUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message report utils module with configuration
bool message_report_utils_init(const MessageReportUtilsConfig& config);

// Handle a message report utils request, returning a response
MessageReportUtilsResponse message_report_utils_handle(const MessageReportUtilsRequest& req);

// Get the current operational status
MessageReportUtilsStatus message_report_utils_getStatus();

// Reset the module to initial state
void message_report_utils_reset();

// Validate input before processing
bool message_report_utils_validate(const std::string& input);

// Process input and return result
std::string message_report_utils_process(const std::string& input);

// Serialize to JSON format
json message_report_utils_toJson(const std::string& input);

// Get runtime statistics
MessageReportUtilsStats message_report_utils_getStats();

// Build config from JSON
MessageReportUtilsConfig message_report_utils_configFromJson(const json& j);

// Serialize config to JSON
json message_report_utils_configToJson(const MessageReportUtilsConfig& config);

} // namespace pcp
