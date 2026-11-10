#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CompressorType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_compressor_stateToString(CompressorType s);
CompressorType media_compressor_stateFromString(const std::string& s);

struct MediaCompressorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaCompressorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaCompressorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaCompressorStatus {
    CompressorType state = CompressorType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaCompressorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media compressor module with configuration
bool media_compressor_init(const MediaCompressorConfig& config);

// Handle a media compressor request, returning a response
MediaCompressorResponse media_compressor_handle(const MediaCompressorRequest& req);

// Get the current operational status
MediaCompressorStatus media_compressor_getStatus();

// Reset the module to initial state
void media_compressor_reset();

// Validate input before processing
bool media_compressor_validate(const std::string& input);

// Process input and return result
std::string media_compressor_process(const std::string& input);

// Serialize to JSON format
json media_compressor_toJson(const std::string& input);

// Get runtime statistics
MediaCompressorStats media_compressor_getStats();

// Build config from JSON
MediaCompressorConfig media_compressor_configFromJson(const json& j);

// Serialize config to JSON
json media_compressor_configToJson(const MediaCompressorConfig& config);

} // namespace pcp
