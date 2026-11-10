#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReactionAggregatorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* reaction_aggregator_stateToString(ReactionAggregatorState s);
ReactionAggregatorState reaction_aggregator_stateFromString(const std::string& s);

struct ReactionAggregatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReactionAggregatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReactionAggregatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReactionAggregatorStatus {
    ReactionAggregatorState state = ReactionAggregatorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReactionAggregatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the reaction aggregator module with configuration
bool reaction_aggregator_init(const ReactionAggregatorConfig& config);

// Handle a reaction aggregator request, returning a response
ReactionAggregatorResponse reaction_aggregator_handle(const ReactionAggregatorRequest& req);

// Get the current operational status
ReactionAggregatorStatus reaction_aggregator_getStatus();

// Reset the module to initial state
void reaction_aggregator_reset();

// Validate input before processing
bool reaction_aggregator_validate(const std::string& input);

// Process input and return result
std::string reaction_aggregator_process(const std::string& input);

// Serialize to JSON format
json reaction_aggregator_toJson(const std::string& input);

// Get runtime statistics
ReactionAggregatorStats reaction_aggregator_getStats();

// Build config from JSON
ReactionAggregatorConfig reaction_aggregator_configFromJson(const json& j);

// Serialize config to JSON
json reaction_aggregator_configToJson(const ReactionAggregatorConfig& config);

} // namespace pcp
