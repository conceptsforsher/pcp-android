#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LiveDraftState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* live_draft_stateToString(LiveDraftState s);
LiveDraftState live_draft_stateFromString(const std::string& s);

struct LiveDraftConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LiveDraftRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LiveDraftResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LiveDraftStatus {
    LiveDraftState state = LiveDraftState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LiveDraftStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the live draft module with configuration
bool live_draft_init(const LiveDraftConfig& config);

// Handle a live draft request, returning a response
LiveDraftResponse live_draft_handle(const LiveDraftRequest& req);

// Get the current operational status
LiveDraftStatus live_draft_getStatus();

// Reset the module to initial state
void live_draft_reset();

// Validate input before processing
bool live_draft_validate(const std::string& input);

// Process input and return result
std::string live_draft_process(const std::string& input);

// Serialize to JSON format
json live_draft_toJson(const std::string& input);

// Get runtime statistics
LiveDraftStats live_draft_getStats();

// Build config from JSON
LiveDraftConfig live_draft_configFromJson(const json& j);

// Serialize config to JSON
json live_draft_configToJson(const LiveDraftConfig& config);

} // namespace pcp
