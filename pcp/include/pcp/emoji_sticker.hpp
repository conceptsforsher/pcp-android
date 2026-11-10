#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EmojiStickerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* emoji_sticker_stateToString(EmojiStickerState s);
EmojiStickerState emoji_sticker_stateFromString(const std::string& s);

struct EmojiStickerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EmojiStickerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EmojiStickerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EmojiStickerStatus {
    EmojiStickerState state = EmojiStickerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EmojiStickerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the emoji sticker module with configuration
bool emoji_sticker_init(const EmojiStickerConfig& config);

// Handle a emoji sticker request, returning a response
EmojiStickerResponse emoji_sticker_handle(const EmojiStickerRequest& req);

// Get the current operational status
EmojiStickerStatus emoji_sticker_getStatus();

// Reset the module to initial state
void emoji_sticker_reset();

// Validate input before processing
bool emoji_sticker_validate(const std::string& input);

// Process input and return result
std::string emoji_sticker_process(const std::string& input);

// Serialize to JSON format
json emoji_sticker_toJson(const std::string& input);

// Get runtime statistics
EmojiStickerStats emoji_sticker_getStats();

// Build config from JSON
EmojiStickerConfig emoji_sticker_configFromJson(const json& j);

// Serialize config to JSON
json emoji_sticker_configToJson(const EmojiStickerConfig& config);

} // namespace pcp
