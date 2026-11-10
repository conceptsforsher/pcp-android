#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EmojiSearchState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* emoji_search_stateToString(EmojiSearchState s);
EmojiSearchState emoji_search_stateFromString(const std::string& s);

struct EmojiSearchConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EmojiSearchRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EmojiSearchResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EmojiSearchStatus {
    EmojiSearchState state = EmojiSearchState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EmojiSearchStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the emoji search module with configuration
bool emoji_search_init(const EmojiSearchConfig& config);

// Handle a emoji search request, returning a response
EmojiSearchResponse emoji_search_handle(const EmojiSearchRequest& req);

// Get the current operational status
EmojiSearchStatus emoji_search_getStatus();

// Reset the module to initial state
void emoji_search_reset();

// Validate input before processing
bool emoji_search_validate(const std::string& input);

// Process input and return result
std::string emoji_search_process(const std::string& input);

// Serialize to JSON format
json emoji_search_toJson(const std::string& input);

// Get runtime statistics
EmojiSearchStats emoji_search_getStats();

// Build config from JSON
EmojiSearchConfig emoji_search_configFromJson(const json& j);

// Serialize config to JSON
json emoji_search_configToJson(const EmojiSearchConfig& config);

} // namespace pcp
