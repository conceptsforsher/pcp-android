#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReadReceiptFormatterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* read_receipt_formatter_stateToString(ReadReceiptFormatterState s);
ReadReceiptFormatterState read_receipt_formatter_stateFromString(const std::string& s);

struct ReadReceiptFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReadReceiptFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReadReceiptFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReadReceiptFormatterStatus {
    ReadReceiptFormatterState state = ReadReceiptFormatterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReadReceiptFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the read receipt formatter module with configuration
bool read_receipt_formatter_init(const ReadReceiptFormatterConfig& config);

// Handle a read receipt formatter request, returning a response
ReadReceiptFormatterResponse read_receipt_formatter_handle(const ReadReceiptFormatterRequest& req);

// Get the current operational status
ReadReceiptFormatterStatus read_receipt_formatter_getStatus();

// Reset the module to initial state
void read_receipt_formatter_reset();

// Validate input before processing
bool read_receipt_formatter_validate(const std::string& input);

// Process input and return result
std::string read_receipt_formatter_process(const std::string& input);

// Serialize to JSON format
json read_receipt_formatter_toJson(const std::string& input);

// Get runtime statistics
ReadReceiptFormatterStats read_receipt_formatter_getStats();

// Build config from JSON
ReadReceiptFormatterConfig read_receipt_formatter_configFromJson(const json& j);

// Serialize config to JSON
json read_receipt_formatter_configToJson(const ReadReceiptFormatterConfig& config);

} // namespace pcp
