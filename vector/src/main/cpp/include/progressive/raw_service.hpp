#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <cstdint>

namespace progressive {

// Raw Service — fetches raw URLs with caching support.
// Used for .well-known discovery, integration manager configs, etc.
//
// Original Kotlin (RawService.kt:22-41):
//   interface RawService {
//       suspend fun getUrl(url: String, cacheStrategy: CacheStrategy): String
//       suspend fun getWellknown(domain: String): String
//       suspend fun clearCache()
//   }

// Cache strategy types, matching original Kotlin sealed class.
//
// Original Kotlin (CacheStrategy.kt):
//   sealed class CacheStrategy {
//       object NoCache : CacheStrategy()
//       data class TtlCache(val validityDurationInMillis: Long,
//           val strict: Boolean) : CacheStrategy()
//       object InfiniteCache : CacheStrategy()
//   }
#ifndef PROGRESSIVE_CACHE_STRATEGY_TYPE_DEFINED
#define PROGRESSIVE_CACHE_STRATEGY_TYPE_DEFINED
enum class CacheStrategyType {
    NO_CACHE = 0,
    TTL_CACHE = 1,
    INFINITE_CACHE = 2
};
#endif

struct CacheStrategy {
    CacheStrategyType type = CacheStrategyType::NO_CACHE;
    int64_t validityDurationMillis = 0;  // For TTL_CACHE
    bool strict = false;                  // For TTL_CACHE: fail on expired cache

    static CacheStrategy noCache() {
        return {CacheStrategyType::NO_CACHE, 0, false};
    }

    static CacheStrategy ttlCache(int64_t millis, bool strictMode) {
        return {CacheStrategyType::TTL_CACHE, millis, strictMode};
    }

    static CacheStrategy infiniteCache() {
        return {CacheStrategyType::INFINITE_CACHE, INT64_MAX, true};
    }
};

// A single cached URL entry.
//
// Original Kotlin (RawCacheEntity):
//   data class RawCacheEntity(
//       val url: String,
//       val data: String,
//       val lastUpdatedTimestamp: Long
//   )
struct RawCacheEntry {
    std::string url;
    std::string data;
    int64_t lastUpdatedTimestamp = 0;  // epoch millis
    bool isValid = false;               // entry has valid data

    // Is the cache still valid given a TTL?
    // Original Kotlin (GetUrlTask.kt:76-79):
    //   isCacheValid = entity != null &&
    //       Date().time < entity.lastUpdatedTimestamp + validityDurationInMillis
    bool isFresh(int64_t validityDurationMillis, int64_t nowMillis) const {
        if (!isValid) return false;
        return nowMillis < lastUpdatedTimestamp + validityDurationMillis;
    }
};

// Result of a getUrl operation.
struct RawGetResult {
    std::string data;              // The fetched/cached content
    bool fromCache = false;        // Was it served from cache?
    bool isExpired = false;        // Was cache expired (used as fallback)?
    int httpStatusCode = 0;        // HTTP status if fetched from network
    std::string errorMessage;      // Error message if request failed
};

// Determine the cache strategy from params.
// Returns true if the URL should be fetched from network (no valid cache).
//
// Original Kotlin (GetUrlTask.kt:52-67):
//   when (params.cacheStrategy) {
//       CacheStrategy.NoCache -> doRequest(url)
//       CacheStrategy.TtlCache -> doRequestWithCache(...)
//       CacheStrategy.InfiniteCache -> doRequestWithCache(Long.MAX_VALUE, true)
//   }
bool shouldFetchFromNetwork(
    const CacheStrategy& strategy,
    const RawCacheEntry& cachedEntry,
    int64_t nowMillis
);


// Serialize a cache entry to JSON for storage.
std::string rawCacheEntryToJson(const RawCacheEntry& entry);

// Parse a cache entry from stored JSON.
RawCacheEntry rawCacheEntryFromJson(const std::string& json);

// Create cache key from URL (simple hash for indexing).
std::string cacheKeyForUrl(const std::string& url);

// ================================================================
// HTTP Request/Response Model (extended)
// ================================================================
//
// Original Kotlin: OkHttp Request/Response usage in RawService
// Replaces Retrofit/OkHttp for raw URL fetching.

enum class HttpMethod {
    GET = 0,
    POST = 1,
    PUT = 2,
    DELETE_ = 3,   // avoid macro conflict
    PATCH = 4,
    HEAD = 5
};

const char* httpMethodToString(HttpMethod method);

// Original Kotlin: OkHttp RetryPolicy / custom retry logic
enum class BackoffStrategy {
    NONE = 0,
    FIXED = 1,      // fixed delay between retries
    LINEAR = 2,     // delay * retryCount
    EXPONENTIAL = 3 // delay * 2^retryCount
};

struct RetryPolicy {
    int maxRetries = 0;
    std::vector<int> retryableStatusCodes;
    BackoffStrategy backoffStrategy = BackoffStrategy::EXPONENTIAL;
    int baseDelayMs = 1000;
    int maxDelayMs = 30000;
};

struct RawRequest {
    HttpMethod method = HttpMethod::GET;
    std::string url;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    int timeoutMs = 30000;
    RetryPolicy retryPolicy;
};

// Original Kotlin: HttpResponse with status, body, headers
struct RawResponse {
    int statusCode = 0;
    std::string statusMessage;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    int64_t requestTimeMs = 0;
    int retryCount = 0;
    bool fromCache = false;

    bool isOk() const { return statusCode >= 200 && statusCode < 300; }
};

// Original Kotlin: OkHttp Request.Builder
RawRequest buildRawRequest(HttpMethod method, const std::string& url,
    const std::unordered_map<std::string, std::string>& headers = {},
    const std::string& body = "", int timeoutMs = 30000);

// Parse an HTTP response from raw bytes (status line + headers + body).
// Original Kotlin: OkHttp Response parsing
RawResponse parseRawResponse(const std::string& rawHttpResponse);

// Add Bearer token to a headers map (mutates).
// Original Kotlin: request.addHeader("Authorization", "Bearer $token")
void addAuthHeader(std::unordered_map<std::string, std::string>& headers,
    const std::string& token);

// Set Content-Type header (mutates).
// Original Kotlin: request.addHeader("Content-Type", contentType)
void addContentTypeHeader(std::unordered_map<std::string, std::string>& headers,
    const std::string& contentType);

// Build URL query string from parameter map.
// Returns "?key1=val1&key2=val2" (with proper %-encoding).
std::string buildQueryString(
    const std::unordered_map<std::string, std::string>& params);

// Build URL-encoded form body from parameter map.
std::string buildFormBody(
    const std::unordered_map<std::string, std::string>& params);

// Build multipart/form-data body.
// Each part is {name, filename, contentType, body}.
struct MultipartPart {
    std::string name;
    std::string filename;       // empty = simple field
    std::string contentType;
    std::string body;           // raw bytes (binary-safe)
};

std::string buildMultipartBody(
    const std::vector<MultipartPart>& parts,
    const std::string& boundary = "");

// ================================================================
// HTTP Constants
// ================================================================

namespace HttpHeaders {
    constexpr const char* AUTHORIZATION   = "Authorization";
    constexpr const char* CONTENT_TYPE    = "Content-Type";
    constexpr const char* ACCEPT          = "Accept";
    constexpr const char* USER_AGENT      = "User-Agent";
    constexpr const char* CONTENT_LENGTH  = "Content-Length";
    constexpr const char* HOST            = "Host";
    constexpr const char* CONNECTION      = "Connection";
    constexpr const char* CACHE_CONTROL   = "Cache-Control";
    constexpr const char* IF_MODIFIED_SINCE = "If-Modified-Since";
    constexpr const char* IF_NONE_MATCH   = "If-None-Match";
    constexpr const char* ETAG            = "ETag";
    constexpr const char* LAST_MODIFIED   = "Last-Modified";
    constexpr const char* LOCATION        = "Location";
    constexpr const char* RETRY_AFTER     = "Retry-After";
    constexpr const char* X_REQUEST_ID    = "X-Request-Id";
    constexpr const char* ACCEPT_ENCODING = "Accept-Encoding";
    constexpr const char* CONTENT_ENCODING = "Content-Encoding";
    constexpr const char* COOKIE          = "Cookie";
    constexpr const char* SET_COOKIE      = "Set-Cookie";
    constexpr const char* REFERER         = "Referer";
    constexpr const char* ORIGIN          = "Origin";
    constexpr const char* X_FORWARDED_FOR = "X-Forwarded-For";
    constexpr const char* X_REAL_IP       = "X-Real-IP";
}

namespace HttpContentTypes {
    constexpr const char* JSON               = "application/json";
    constexpr const char* FORM_URLENCODED    = "application/x-www-form-urlencoded";
    constexpr const char* MULTIPART_FORM     = "multipart/form-data";
    constexpr const char* TEXT_PLAIN         = "text/plain";
    constexpr const char* TEXT_HTML          = "text/html";
    constexpr const char* OCTET_STREAM       = "application/octet-stream";
    constexpr const char* MATRIX_JSON        = "application/json";  // Matrix API
    constexpr const char* PROBLEM_JSON       = "application/problem+json";
    constexpr const char* APPLICATION_XML    = "application/xml";
    constexpr const char* TEXT_XML           = "text/xml";
    constexpr const char* IMAGE_PNG          = "image/png";
    constexpr const char* IMAGE_JPEG         = "image/jpeg";
    constexpr const char* IMAGE_WEBP         = "image/webp";
    constexpr const char* AUDIO_OGG          = "audio/ogg";
    constexpr const char* VIDEO_MP4          = "video/mp4";
}

namespace HttpStatusCodes {
    // 2xx Success
    constexpr int OK          = 200;
    constexpr int CREATED     = 201;
    constexpr int ACCEPTED    = 202;
    constexpr int NO_CONTENT  = 204;

    // 3xx Redirection
    constexpr int MOVED_PERMANENTLY  = 301;
    constexpr int FOUND              = 302;
    constexpr int NOT_MODIFIED       = 304;
    constexpr int TEMPORARY_REDIRECT = 307;
    constexpr int PERMANENT_REDIRECT = 308;

    // 4xx Client Errors
    constexpr int BAD_REQUEST     = 400;
    constexpr int UNAUTHORIZED    = 401;
    constexpr int FORBIDDEN       = 403;
    constexpr int NOT_FOUND       = 404;
    constexpr int METHOD_NOT_ALLOWED = 405;
    constexpr int CONFLICT        = 409;
    constexpr int GONE            = 410;
    constexpr int TOO_MANY_REQUESTS = 429;

    // 5xx Server Errors
    constexpr int INTERNAL_SERVER_ERROR = 500;
    constexpr int BAD_GATEWAY           = 502;
    constexpr int SERVICE_UNAVAILABLE   = 503;
    constexpr int GATEWAY_TIMEOUT       = 504;

    inline bool isSuccess(int code)       { return code >= 200 && code < 300; }
    inline bool isClientError(int code)   { return code >= 400 && code < 500; }
    inline bool isServerError(int code)   { return code >= 500 && code < 600; }
    inline bool isRedirect(int code)      { return code >= 300 && code < 400; }
    inline bool isRetryable(int code) {
        return code == 429 || code >= 500;
    }
}

// Check if a request should be retried given the response and attempt count.
bool applyRetryPolicy(const RetryPolicy& policy, int statusCode, int retryCount);

// Calculate the delay before the next retry attempt.
int calcRetryDelay(const RetryPolicy& policy, int retryCount);

// ================================================================
// Request Logger
// ================================================================

struct RequestLog {
    RawRequest request;
    RawResponse response;
    int64_t timestampMs = 0;
    int64_t durationMs = 0;
    int retryCount = 0;
};

class RequestLogger {
public:
    static constexpr size_t MAX_LOG_ENTRIES = 256;

    void addLog(const RequestLog& log);
    std::vector<RequestLog> getLogs() const;
    std::vector<RequestLog> getRecentLogs(size_t count) const;
    void clear();
    size_t size() const;

private:
    std::deque<RequestLog> logs_;
};

// ================================================================
// Request Metrics
// ================================================================

struct RequestMetrics {
    int totalRequests = 0;
    int successfulRequests = 0;
    int failedRequests = 0;
    double avgLatencyMs = 0.0;
    double p50LatencyMs = 0.0;
    double p95LatencyMs = 0.0;
    double p99LatencyMs = 0.0;
    int64_t firstRequestTs = 0;
    int64_t lastRequestTs = 0;
};

// Compute aggregate metrics from a set of request logs.
RequestMetrics computeRequestMetrics(const std::vector<RequestLog>& logs);

} // namespace progressive
