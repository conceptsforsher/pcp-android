#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ReadReceiptDisplayState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* read_receipt_display_stateToString(ReadReceiptDisplayState s);
ReadReceiptDisplayState read_receipt_display_stateFromString(const std::string& s);

struct ReadReceiptDisplayConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReadReceiptDisplayRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReadReceiptDisplayResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReadReceiptDisplayStatus {
    ReadReceiptDisplayState state = ReadReceiptDisplayState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReadReceiptDisplayStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the read receipt display module with configuration
bool read_receipt_display_init(const ReadReceiptDisplayConfig& config);

// Handle a read receipt display request, returning a response
ReadReceiptDisplayResponse read_receipt_display_handle(const ReadReceiptDisplayRequest& req);

// Get the current operational status
ReadReceiptDisplayStatus read_receipt_display_getStatus();

// Reset the module to initial state
void read_receipt_display_reset();

// Validate input before processing
bool read_receipt_display_validate(const std::string& input);

// Process input and return result
std::string read_receipt_display_process(const std::string& input);

// Serialize to JSON format
json read_receipt_display_toJson(const std::string& input);

// Get runtime statistics
ReadReceiptDisplayStats read_receipt_display_getStats();

// Build config from JSON
ReadReceiptDisplayConfig read_receipt_display_configFromJson(const json& j);

// Serialize config to JSON
json read_receipt_display_configToJson(const ReadReceiptDisplayConfig& config);
