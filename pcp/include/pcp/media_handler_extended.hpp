#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class HandlerExtendedType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_handler_extended_stateToString(HandlerExtendedType s);
HandlerExtendedType media_handler_extended_stateFromString(const std::string& s);

struct MediaHandlerExtendedConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaHandlerExtendedRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaHandlerExtendedResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaHandlerExtendedStatus {
    HandlerExtendedType state = HandlerExtendedType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaHandlerExtendedStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media handler extended module with configuration
bool media_handler_extended_init(const MediaHandlerExtendedConfig& config);

// Handle a media handler extended request, returning a response
MediaHandlerExtendedResponse media_handler_extended_handle(const MediaHandlerExtendedRequest& req);

// Get the current operational status
MediaHandlerExtendedStatus media_handler_extended_getStatus();

// Reset the module to initial state
void media_handler_extended_reset();

// Validate input before processing
bool media_handler_extended_validate(const std::string& input);

// Process input and return result
std::string media_handler_extended_process(const std::string& input);

// Serialize to JSON format
json media_handler_extended_toJson(const std::string& input);

// Get runtime statistics
MediaHandlerExtendedStats media_handler_extended_getStats();

// Build config from JSON
MediaHandlerExtendedConfig media_handler_extended_configFromJson(const json& j);

// Serialize config to JSON
json media_handler_extended_configToJson(const MediaHandlerExtendedConfig& config);
