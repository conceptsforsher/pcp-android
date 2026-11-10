#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ScannerType {
    UNKNOWN = 0,
    IMAGE = 1,
    VIDEO = 2,
    AUDIO = 3,
    FILE = 4,
    STICKER = 5
};

const char* media_scanner_stateToString(ScannerType s);
ScannerType media_scanner_stateFromString(const std::string& s);

struct MediaScannerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MediaScannerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MediaScannerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MediaScannerStatus {
    ScannerType state = ScannerType::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MediaScannerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the media scanner module with configuration
bool media_scanner_init(const MediaScannerConfig& config);

// Handle a media scanner request, returning a response
MediaScannerResponse media_scanner_handle(const MediaScannerRequest& req);

// Get the current operational status
MediaScannerStatus media_scanner_getStatus();

// Reset the module to initial state
void media_scanner_reset();

// Validate input before processing
bool media_scanner_validate(const std::string& input);

// Process input and return result
std::string media_scanner_process(const std::string& input);

// Serialize to JSON format
json media_scanner_toJson(const std::string& input);

// Get runtime statistics
MediaScannerStats media_scanner_getStats();

// Build config from JSON
MediaScannerConfig media_scanner_configFromJson(const json& j);

// Serialize config to JSON
json media_scanner_configToJson(const MediaScannerConfig& config);

} // namespace pcp
