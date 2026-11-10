#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SignatureUtilsState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_signature_utils_stateToString(SignatureUtilsState s);
SignatureUtilsState event_signature_utils_stateFromString(const std::string& s);

struct EventSignatureUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventSignatureUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventSignatureUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventSignatureUtilsStatus {
    SignatureUtilsState state = SignatureUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventSignatureUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event signature utils module with configuration
bool event_signature_utils_init(const EventSignatureUtilsConfig& config);

// Handle a event signature utils request, returning a response
EventSignatureUtilsResponse event_signature_utils_handle(const EventSignatureUtilsRequest& req);

// Get the current operational status
EventSignatureUtilsStatus event_signature_utils_getStatus();

// Reset the module to initial state
void event_signature_utils_reset();

// Validate input before processing
bool event_signature_utils_validate(const std::string& input);

// Process input and return result
std::string event_signature_utils_process(const std::string& input);

// Serialize to JSON format
json event_signature_utils_toJson(const std::string& input);

// Get runtime statistics
EventSignatureUtilsStats event_signature_utils_getStats();

// Build config from JSON
EventSignatureUtilsConfig event_signature_utils_configFromJson(const json& j);

// Serialize config to JSON
json event_signature_utils_configToJson(const EventSignatureUtilsConfig& config);

} // namespace pcp
