#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SearchState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_search_stateToString(SearchState s);
SearchState event_search_stateFromString(const std::string& s);

struct EventSearchConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventSearchRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventSearchResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventSearchStatus {
    SearchState state = SearchState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventSearchStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event search module with configuration
bool event_search_init(const EventSearchConfig& config);

// Handle a event search request, returning a response
EventSearchResponse event_search_handle(const EventSearchRequest& req);

// Get the current operational status
EventSearchStatus event_search_getStatus();

// Reset the module to initial state
void event_search_reset();

// Validate input before processing
bool event_search_validate(const std::string& input);

// Process input and return result
std::string event_search_process(const std::string& input);

// Serialize to JSON format
json event_search_toJson(const std::string& input);

// Get runtime statistics
EventSearchStats event_search_getStats();

// Build config from JSON
EventSearchConfig event_search_configFromJson(const json& j);

// Serialize config to JSON
json event_search_configToJson(const EventSearchConfig& config);

} // namespace pcp
