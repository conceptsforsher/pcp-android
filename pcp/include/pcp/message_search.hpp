#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SearchPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_search_stateToString(SearchPhase s);
SearchPhase message_search_stateFromString(const std::string& s);

struct MessageSearchConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageSearchRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageSearchResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageSearchStatus {
    SearchPhase state = SearchPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageSearchStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message search module with configuration
bool message_search_init(const MessageSearchConfig& config);

// Handle a message search request, returning a response
MessageSearchResponse message_search_handle(const MessageSearchRequest& req);

// Get the current operational status
MessageSearchStatus message_search_getStatus();

// Reset the module to initial state
void message_search_reset();

// Validate input before processing
bool message_search_validate(const std::string& input);

// Process input and return result
std::string message_search_process(const std::string& input);

// Serialize to JSON format
json message_search_toJson(const std::string& input);

// Get runtime statistics
MessageSearchStats message_search_getStats();

// Build config from JSON
MessageSearchConfig message_search_configFromJson(const json& j);

// Serialize config to JSON
json message_search_configToJson(const MessageSearchConfig& config);

} // namespace pcp
