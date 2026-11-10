#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class AttachmentHandlerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* attachment_handler_stateToString(AttachmentHandlerState s);
AttachmentHandlerState attachment_handler_stateFromString(const std::string& s);

struct AttachmentHandlerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct AttachmentHandlerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct AttachmentHandlerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct AttachmentHandlerStatus {
    AttachmentHandlerState state = AttachmentHandlerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct AttachmentHandlerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the attachment handler module with configuration
bool attachment_handler_init(const AttachmentHandlerConfig& config);

// Handle a attachment handler request, returning a response
AttachmentHandlerResponse attachment_handler_handle(const AttachmentHandlerRequest& req);

// Get the current operational status
AttachmentHandlerStatus attachment_handler_getStatus();

// Reset the module to initial state
void attachment_handler_reset();

// Validate input before processing
bool attachment_handler_validate(const std::string& input);

// Process input and return result
std::string attachment_handler_process(const std::string& input);

// Serialize to JSON format
json attachment_handler_toJson(const std::string& input);

// Get runtime statistics
AttachmentHandlerStats attachment_handler_getStats();

// Build config from JSON
AttachmentHandlerConfig attachment_handler_configFromJson(const json& j);

// Serialize config to JSON
json attachment_handler_configToJson(const AttachmentHandlerConfig& config);

} // namespace pcp
