#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

// ==== Lightweight HTTP/1.1 Client (NDK POSIX sockets) ====
//
// Pure C++ HTTP client for Android NDK. No external dependencies.
// Uses POSIX sockets (available on all Android API levels).
// Supports: GET, POST, PUT, DELETE, custom headers, timeouts.
//
// This replaces Retrofit/OkHttp for Matrix API calls in the native layer.

struct HttpRequest {
    std::string method;                      // "GET", "POST", "PUT", "DELETE"
    std::string url;                         // Full URL including https://
    std::string body;                        // Request body (JSON for Matrix API)
    std::unordered_map<std::string, std::string> headers;
    int timeoutMs = 30000;                   // Connection + read timeout
    bool followRedirects = true;
};

struct HttpResponse {
    int statusCode = 0;                      // HTTP status code (200, 401, etc.)
    std::string body;                        // Response body
    std::unordered_map<std::string, std::string> headers;
    bool success = false;                    // true if request was sent and response received
    std::string errorMessage;                // Empty on success

    bool isOk() const { return statusCode >= 200 && statusCode < 300; }
    bool isClientError() const { return statusCode >= 400 && statusCode < 500; }
    bool isServerError() const { return statusCode >= 500; }
};

// ==== URL Parsing ====

struct ParsedUrl {
    std::string scheme;          // "https" or "http"
    std::string host;            // "matrix.example.org"
    int port = 443;              // default: 443 for https, 80 for http
    std::string path;            // "/_matrix/client/r0/sync"
    bool valid = false;
};

// Parse URL into components. Handles matrix:// and https:// schemes.
ParsedUrl parseUrl(const std::string& url);

// ==== HTTP Client ====

// Execute a single HTTP request synchronously.
// Handles TLS via Android's built-in SSL (JNI bridge to javax.net.ssl).
// For pure C++ TLS, link against OpenSSL/BoringSSL.
HttpResponse httpExecute(const HttpRequest& request);

// Convenience: GET request
inline HttpResponse httpGet(const std::string& url,
    const std::unordered_map<std::string, std::string>& headers = {},
    int timeoutMs = 30000)
{
    HttpRequest req{"GET", url, "", headers, timeoutMs};
    return httpExecute(req);
}

// Convenience: POST with JSON body
inline HttpResponse httpPost(const std::string& url, const std::string& jsonBody,
    const std::unordered_map<std::string, std::string>& extraHeaders = {},
    int timeoutMs = 30000)
{
    HttpRequest req{"POST", url, jsonBody, {{"Content-Type", "application/json"}}, timeoutMs};
    for (const auto& [k, v] : extraHeaders) req.headers[k] = v;
    return httpExecute(req);
}

// Convenience: PUT with JSON body
inline HttpResponse httpPut(const std::string& url, const std::string& jsonBody,
    int timeoutMs = 30000)
{
    HttpRequest req{"PUT", url, jsonBody, {{"Content-Type", "application/json"}}, timeoutMs};
    return httpExecute(req);
}

// ==== Matrix API Helpers ====

// Build full Matrix API URL from homeserver base and endpoint path.
// Example: buildMatrixUrl("https://matrix.example.org", "/_matrix/client/r0/sync")
//   → "https://matrix.example.org/_matrix/client/r0/sync"
inline std::string buildMatrixUrl(const std::string& homeserverBase, const std::string& path) {
    std::string url = homeserverBase;
    if (!url.empty() && url.back() == '/') url.pop_back();
    if (!path.empty() && path[0] != '/') url += '/';
    url += path;
    return url;
}

// Add Matrix access token to request headers.
inline void addAuthHeader(HttpRequest& req, const std::string& accessToken) {
    req.headers["Authorization"] = "Bearer " + accessToken;
}

// Build URL-encoded form body from key-value pairs.
std::string buildFormBody(const std::unordered_map<std::string, std::string>& params);

// Extract rate limit info from response headers.
struct RateLimitInfo {
    int limit = -1;          // X-RateLimit-Limit
    int remaining = -1;      // X-RateLimit-Remaining
    int64_t resetMs = 0;     // X-RateLimit-Reset (epoch seconds → millis)
};
RateLimitInfo parseRateLimitHeaders(const HttpResponse& response);

// ==== Response Parsing Helpers ====

// Extract JSON error from Matrix error response.
// Format: {"errcode":"M_FORBIDDEN","error":"User not in room"}
struct MatrixErrorResponse {
    std::string errcode;     // "M_FORBIDDEN", "M_UNKNOWN_TOKEN", etc.
    std::string error;       // Human-readable error message
    int retryAfterMs = 0;    // From "retry_after_ms" field or 0
};

MatrixErrorResponse parseMatrixError(const std::string& responseBody);

// Check if an HTTP response indicates a rate limit (429 Too Many Requests).
inline bool isRateLimited(const HttpResponse& resp) { return resp.statusCode == 429; }

} // namespace progressive
