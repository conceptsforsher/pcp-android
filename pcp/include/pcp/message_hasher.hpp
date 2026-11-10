#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class HasherPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_hasher_stateToString(HasherPhase s);
HasherPhase message_hasher_stateFromString(const std::string& s);

struct MessageHasherConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageHasherRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageHasherResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageHasherStatus {
    HasherPhase state = HasherPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageHasherStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message hasher module with configuration
bool message_hasher_init(const MessageHasherConfig& config);

// Handle a message hasher request, returning a response
MessageHasherResponse message_hasher_handle(const MessageHasherRequest& req);

// Get the current operational status
MessageHasherStatus message_hasher_getStatus();

// Reset the module to initial state
void message_hasher_reset();

// Validate input before processing
bool message_hasher_validate(const std::string& input);

// Process input and return result
std::string message_hasher_process(const std::string& input);

// Serialize to JSON format
json message_hasher_toJson(const std::string& input);

// Get runtime statistics
MessageHasherStats message_hasher_getStats();

// Build config from JSON
MessageHasherConfig message_hasher_configFromJson(const json& j);

// Serialize config to JSON
json message_hasher_configToJson(const MessageHasherConfig& config);

} // namespace pcp
