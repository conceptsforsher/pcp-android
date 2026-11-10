#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FormatUtilsPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_format_utils_stateToString(FormatUtilsPhase s);
FormatUtilsPhase message_format_utils_stateFromString(const std::string& s);

struct MessageFormatUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageFormatUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageFormatUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageFormatUtilsStatus {
    FormatUtilsPhase state = FormatUtilsPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageFormatUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message format utils module with configuration
bool message_format_utils_init(const MessageFormatUtilsConfig& config);

// Handle a message format utils request, returning a response
MessageFormatUtilsResponse message_format_utils_handle(const MessageFormatUtilsRequest& req);

// Get the current operational status
MessageFormatUtilsStatus message_format_utils_getStatus();

// Reset the module to initial state
void message_format_utils_reset();

// Validate input before processing
bool message_format_utils_validate(const std::string& input);

// Process input and return result
std::string message_format_utils_process(const std::string& input);

// Serialize to JSON format
json message_format_utils_toJson(const std::string& input);

// Get runtime statistics
MessageFormatUtilsStats message_format_utils_getStats();

// Build config from JSON
MessageFormatUtilsConfig message_format_utils_configFromJson(const json& j);

// Serialize config to JSON
json message_format_utils_configToJson(const MessageFormatUtilsConfig& config);

} // namespace pcp
