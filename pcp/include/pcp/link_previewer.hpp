#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LinkPreviewerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* link_previewer_stateToString(LinkPreviewerState s);
LinkPreviewerState link_previewer_stateFromString(const std::string& s);

struct LinkPreviewerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LinkPreviewerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LinkPreviewerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LinkPreviewerStatus {
    LinkPreviewerState state = LinkPreviewerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LinkPreviewerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the link previewer module with configuration
bool link_previewer_init(const LinkPreviewerConfig& config);

// Handle a link previewer request, returning a response
LinkPreviewerResponse link_previewer_handle(const LinkPreviewerRequest& req);

// Get the current operational status
LinkPreviewerStatus link_previewer_getStatus();

// Reset the module to initial state
void link_previewer_reset();

// Validate input before processing
bool link_previewer_validate(const std::string& input);

// Process input and return result
std::string link_previewer_process(const std::string& input);

// Serialize to JSON format
json link_previewer_toJson(const std::string& input);

// Get runtime statistics
LinkPreviewerStats link_previewer_getStats();

// Build config from JSON
LinkPreviewerConfig link_previewer_configFromJson(const json& j);

// Serialize config to JSON
json link_previewer_configToJson(const LinkPreviewerConfig& config);

} // namespace pcp
