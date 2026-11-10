#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SearcherPhase {
    IDLE = 0,
    QUEUED = 1,
    SENDING = 2,
    SENT = 3,
    DELIVERED = 4,
    FAILED = 5
};

const char* message_searcher_stateToString(SearcherPhase s);
SearcherPhase message_searcher_stateFromString(const std::string& s);

struct MessageSearcherConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MessageSearcherRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MessageSearcherResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MessageSearcherStatus {
    SearcherPhase state = SearcherPhase::IDLE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MessageSearcherStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the message searcher module with configuration
bool message_searcher_init(const MessageSearcherConfig& config);

// Handle a message searcher request, returning a response
MessageSearcherResponse message_searcher_handle(const MessageSearcherRequest& req);

// Get the current operational status
MessageSearcherStatus message_searcher_getStatus();

// Reset the module to initial state
void message_searcher_reset();

// Validate input before processing
bool message_searcher_validate(const std::string& input);

// Process input and return result
std::string message_searcher_process(const std::string& input);

// Serialize to JSON format
json message_searcher_toJson(const std::string& input);

// Get runtime statistics
MessageSearcherStats message_searcher_getStats();

// Build config from JSON
MessageSearcherConfig message_searcher_configFromJson(const json& j);

// Serialize config to JSON
json message_searcher_configToJson(const MessageSearcherConfig& config);

} // namespace pcp
