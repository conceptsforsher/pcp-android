#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MarkdownParserState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* markdown_parser_stateToString(MarkdownParserState s);
MarkdownParserState markdown_parser_stateFromString(const std::string& s);

struct MarkdownParserConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MarkdownParserRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MarkdownParserResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MarkdownParserStatus {
    MarkdownParserState state = MarkdownParserState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MarkdownParserStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the markdown parser module with configuration
bool markdown_parser_init(const MarkdownParserConfig& config);

// Handle a markdown parser request, returning a response
MarkdownParserResponse markdown_parser_handle(const MarkdownParserRequest& req);

// Get the current operational status
MarkdownParserStatus markdown_parser_getStatus();

// Reset the module to initial state
void markdown_parser_reset();

// Validate input before processing
bool markdown_parser_validate(const std::string& input);

// Process input and return result
std::string markdown_parser_process(const std::string& input);

// Serialize to JSON format
json markdown_parser_toJson(const std::string& input);

// Get runtime statistics
MarkdownParserStats markdown_parser_getStats();

// Build config from JSON
MarkdownParserConfig markdown_parser_configFromJson(const json& j);

// Serialize config to JSON
json markdown_parser_configToJson(const MarkdownParserConfig& config);

} // namespace pcp
