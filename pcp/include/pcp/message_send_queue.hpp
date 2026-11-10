#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SendQueuePhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_send_queue_stateToString(SendQueuePhase s);
SendQueuePhase message_send_queue_stateFromString(const std::string& s);

struct MessageSendQueueConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageSendQueueRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageSendQueueResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageSendQueueStatus {
    SendQueuePhase state = SendQueuePhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageSendQueueStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message send queue module with configuration
bool message_send_queue_init(const MessageSendQueueConfig& config);

// Handle a message send queue request, returning a response
MessageSendQueueResponse message_send_queue_handle(const MessageSendQueueRequest& req);

// Get the current operational status
MessageSendQueueStatus message_send_queue_getStatus();

// Reset the module to initial state
void message_send_queue_reset();

// Validate input before processing
bool message_send_queue_validate(const std::string& input);

// Process input and return result
std::string message_send_queue_process(const std::string& input);

// Serialize to JSON format
json message_send_queue_toJson(const std::string& input);

// Get runtime statistics
MessageSendQueueStats message_send_queue_getStats();

// Build config from JSON
MessageSendQueueConfig message_send_queue_configFromJson(const json& j);

// Serialize config to JSON
json message_send_queue_configToJson(const MessageSendQueueConfig& config);

} // namespace pcp
