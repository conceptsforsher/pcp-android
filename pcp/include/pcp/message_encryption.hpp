#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EncryptionPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_encryption_stateToString(EncryptionPhase s);
EncryptionPhase message_encryption_stateFromString(const std::string& s);

struct MessageEncryptionConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageEncryptionRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageEncryptionResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageEncryptionStatus {
    EncryptionPhase state = EncryptionPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageEncryptionStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message encryption module with configuration
bool message_encryption_init(const MessageEncryptionConfig& config);

// Handle a message encryption request, returning a response
MessageEncryptionResponse message_encryption_handle(const MessageEncryptionRequest& req);

// Get the current operational status
MessageEncryptionStatus message_encryption_getStatus();

// Reset the module to initial state
void message_encryption_reset();

// Validate input before processing
bool message_encryption_validate(const std::string& input);

// Process input and return result
std::string message_encryption_process(const std::string& input);

// Serialize to JSON format
json message_encryption_toJson(const std::string& input);

// Get runtime statistics
MessageEncryptionStats message_encryption_getStats();

// Build config from JSON
MessageEncryptionConfig message_encryption_configFromJson(const json& j);

// Serialize config to JSON
json message_encryption_configToJson(const MessageEncryptionConfig& config);

} // namespace pcp
