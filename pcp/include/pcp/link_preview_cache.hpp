#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LinkPreviewCacheState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* link_preview_cache_stateToString(LinkPreviewCacheState s);
LinkPreviewCacheState link_preview_cache_stateFromString(const std::string& s);

struct LinkPreviewCacheConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LinkPreviewCacheRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LinkPreviewCacheResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LinkPreviewCacheStatus {
    LinkPreviewCacheState state = LinkPreviewCacheState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LinkPreviewCacheStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the link preview cache module with configuration
bool link_preview_cache_init(const LinkPreviewCacheConfig& config);

// Handle a link preview cache request, returning a response
LinkPreviewCacheResponse link_preview_cache_handle(const LinkPreviewCacheRequest& req);

// Get the current operational status
LinkPreviewCacheStatus link_preview_cache_getStatus();

// Reset the module to initial state
void link_preview_cache_reset();

// Validate input before processing
bool link_preview_cache_validate(const std::string& input);

// Process input and return result
std::string link_preview_cache_process(const std::string& input);

// Serialize to JSON format
json link_preview_cache_toJson(const std::string& input);

// Get runtime statistics
LinkPreviewCacheStats link_preview_cache_getStats();

// Build config from JSON
LinkPreviewCacheConfig link_preview_cache_configFromJson(const json& j);

// Serialize config to JSON
json link_preview_cache_configToJson(const LinkPreviewCacheConfig& config);

} // namespace pcp
