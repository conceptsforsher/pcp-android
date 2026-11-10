#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WebSearchState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* web_search_stateToString(WebSearchState s);
WebSearchState web_search_stateFromString(const std::string& s);

struct WebSearchConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WebSearchRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WebSearchResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WebSearchStatus {
    WebSearchState state = WebSearchState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WebSearchStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the web search module with configuration
bool web_search_init(const WebSearchConfig& config);

// Handle a web search request, returning a response
WebSearchResponse web_search_handle(const WebSearchRequest& req);

// Get the current operational status
WebSearchStatus web_search_getStatus();

// Reset the module to initial state
void web_search_reset();

// Validate input before processing
bool web_search_validate(const std::string& input);

// Process input and return result
std::string web_search_process(const std::string& input);

// Serialize to JSON format
json web_search_toJson(const std::string& input);

// Get runtime statistics
WebSearchStats web_search_getStats();

// Build config from JSON
WebSearchConfig web_search_configFromJson(const json& j);

// Serialize config to JSON
json web_search_configToJson(const WebSearchConfig& config);

} // namespace pcp
