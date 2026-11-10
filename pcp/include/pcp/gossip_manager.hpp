#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class GossipManagerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* gossip_manager_stateToString(GossipManagerState s);
GossipManagerState gossip_manager_stateFromString(const std::string& s);

struct GossipManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct GossipManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct GossipManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct GossipManagerStatus {
    GossipManagerState state = GossipManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct GossipManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the gossip manager module with configuration
bool gossip_manager_init(const GossipManagerConfig& config);

// Handle a gossip manager request, returning a response
GossipManagerResponse gossip_manager_handle(const GossipManagerRequest& req);

// Get the current operational status
GossipManagerStatus gossip_manager_getStatus();

// Reset the module to initial state
void gossip_manager_reset();

// Validate input before processing
bool gossip_manager_validate(const std::string& input);

// Process input and return result
std::string gossip_manager_process(const std::string& input);

// Serialize to JSON format
json gossip_manager_toJson(const std::string& input);

// Get runtime statistics
GossipManagerStats gossip_manager_getStats();

// Build config from JSON
GossipManagerConfig gossip_manager_configFromJson(const json& j);

// Serialize config to JSON
json gossip_manager_configToJson(const GossipManagerConfig& config);

} // namespace pcp
