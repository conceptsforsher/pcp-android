#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class PcpStubsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* pcp_stubs_stateToString(PcpStubsState s);
PcpStubsState pcp_stubs_stateFromString(const std::string& s);

struct PcpStubsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PcpStubsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PcpStubsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PcpStubsStatus {
    PcpStubsState state = PcpStubsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PcpStubsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the pcp stubs module with configuration
bool pcp_stubs_init(const PcpStubsConfig& config);

// Handle a pcp stubs request, returning a response
PcpStubsResponse pcp_stubs_handle(const PcpStubsRequest& req);

// Get the current operational status
PcpStubsStatus pcp_stubs_getStatus();

// Reset the module to initial state
void pcp_stubs_reset();

// Validate input before processing
bool pcp_stubs_validate(const std::string& input);

// Process input and return result
std::string pcp_stubs_process(const std::string& input);

// Serialize to JSON format
json pcp_stubs_toJson(const std::string& input);

// Get runtime statistics
PcpStubsStats pcp_stubs_getStats();

// Build config from JSON
PcpStubsConfig pcp_stubs_configFromJson(const json& j);

// Serialize config to JSON
json pcp_stubs_configToJson(const PcpStubsConfig& config);
