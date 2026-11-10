#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EditPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_edit_stateToString(EditPhase s);
EditPhase message_edit_stateFromString(const std::string& s);

struct MessageEditConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageEditRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageEditResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageEditStatus {
    EditPhase state = EditPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageEditStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message edit module with configuration
bool message_edit_init(const MessageEditConfig& config);

// Handle a message edit request, returning a response
MessageEditResponse message_edit_handle(const MessageEditRequest& req);

// Get the current operational status
MessageEditStatus message_edit_getStatus();

// Reset the module to initial state
void message_edit_reset();

// Validate input before processing
bool message_edit_validate(const std::string& input);

// Process input and return result
std::string message_edit_process(const std::string& input);

// Serialize to JSON format
json message_edit_toJson(const std::string& input);

// Get runtime statistics
MessageEditStats message_edit_getStats();

// Build config from JSON
MessageEditConfig message_edit_configFromJson(const json& j);

// Serialize config to JSON
json message_edit_configToJson(const MessageEditConfig& config);

} // namespace pcp
