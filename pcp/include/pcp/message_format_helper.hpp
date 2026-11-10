#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FormatHelperPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_format_helper_stateToString(FormatHelperPhase s);
FormatHelperPhase message_format_helper_stateFromString(const std::string& s);

struct MessageFormatHelperConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageFormatHelperRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageFormatHelperResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageFormatHelperStatus {
    FormatHelperPhase state = FormatHelperPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageFormatHelperStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message format helper module with configuration
bool message_format_helper_init(const MessageFormatHelperConfig& config);

// Handle a message format helper request, returning a response
MessageFormatHelperResponse message_format_helper_handle(const MessageFormatHelperRequest& req);

// Get the current operational status
MessageFormatHelperStatus message_format_helper_getStatus();

// Reset the module to initial state
void message_format_helper_reset();

// Validate input before processing
bool message_format_helper_validate(const std::string& input);

// Process input and return result
std::string message_format_helper_process(const std::string& input);

// Serialize to JSON format
json message_format_helper_toJson(const std::string& input);

// Get runtime statistics
MessageFormatHelperStats message_format_helper_getStats();

// Build config from JSON
MessageFormatHelperConfig message_format_helper_configFromJson(const json& j);

// Serialize config to JSON
json message_format_helper_configToJson(const MessageFormatHelperConfig& config);

} // namespace pcp
