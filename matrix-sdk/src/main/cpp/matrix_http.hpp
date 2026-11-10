#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <nlohmann/json.hpp>

namespace matrix_sdk {

using json = nlohmann::json;

// ─────────────────────────────────────────────────────────────
//  Matrix HTTP client with retry, rate limiting, authentication
//  Equivalent to OkHttp + Retrofit interceptor patterns
// ─────────────────────────────────────────────────────────────

// ── HTTP methods ──

enum class HttpMethod { GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS };

inline std::string httpMethodToString(HttpMethod method) {
    switch (method) {
        case HttpMethod::GET:    return "GET";
        case HttpMethod::POST:   return "POST";
        case HttpMethod::PUT:    return "PUT";
        case HttpMethod::DELETE: return "DELETE";
        case HttpMethod::PATCH:  return "PATCH";
        case HttpMethod::HEAD:   return "HEAD";
        case HttpMethod::OPTIONS: return "OPTIONS";
    }
    return "GET";
}

// ── HTTP Request ──

struct HttpHeader {
    std::string name;
    std::string value;
};

struct HttpRequest {
    HttpMethod method = HttpMethod::GET;
    std::string url;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> queryParams;
    std::string body;
    std::vector<uint8_t> binaryBody;
    std::string contentType;
    int timeoutMs = 30000;
    int maxRetries = 0;
    // Request metadata for tracing
    std::string requestId;
    std::string endpoint;
    int64_t createdAt = 0;
};

// ── HTTP Response ──

struct HttpResponse {
    int statusCode = 0;
    std::string body;
    std::vector<uint8_t> binaryBody;
    std::map<std::string, std::string> headers;
    bool success = false;
    std::string error;
    int64_t latencyMs = 0;
    // Rate limit info from headers
    int retryAfterMs = 0;
    int rateLimitRemaining = -1;
    int rateLimitLimit = -1;
    int64_t rateLimitResetMs = 0;
};

// ── HTTP Response listener ──

using HttpResponseCallback = std::function<void(const HttpResponse&)>;
using HttpErrorCallback = std::function<void(const std::string&, int, const std::string&)>;
using HttpProgressCallback = std::function<void(int64_t downloaded, int64_t total)>;

// ── Rate limiting ──

struct RateLimitRule {
    std::string endpoint;         // exact or regex
    int maxRequestsPerSecond = 10;
    int burstSize = 20;
    bool perEndpoint = false;
};

class RateLimitTracker {
public:
    RateLimitTracker() = default;

    bool tryAcquire(const std::string& endpoint) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto now = std::chrono::steady_clock::now();

        auto& window = m_windows[endpoint];
        auto oneSecond = std::chrono::seconds(1);

        // Reset window if expired
        if (now - window.start > oneSecond) {
            window.start = now;
            window.count = 0;
            window.tokens = window.burstSize;
        }

        // Token bucket
        if (window.tokens > 0) {
            window.tokens--;
            window.count++;
            return true;
        }

        // Check global limit
        auto bucketIt = m_buckets.find("__global__");
        if (bucketIt != m_buckets.end()) {
            if (now - bucketIt->second.start > oneSecond) {
                bucketIt->second.start = now;
                bucketIt->second.count = 0;
            }
            if (bucketIt->second.count >= m_globalMaxPerSecond) return false;
            bucketIt->second.count++;
        }

        return false;
    }

    void updateFromResponse(const std::string& endpoint,
                            int retryAfterMs, int remaining, int limit,
                            int64_t resetMs) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (retryAfterMs > 0) {
            m_retryAfter[endpoint] = std::chrono::steady_clock::now()
                + std::chrono::milliseconds(retryAfterMs);
        }
        if (remaining >= 0) {
            m_limitRemaining[endpoint] = remaining;
        }
        if (limit > 0) {
            m_limitLimit[endpoint] = limit;
        }
        if (resetMs > 0) {
            m_limitResetMs[endpoint] = resetMs;
        }
    }

    int64_t timeUntilAvailable(const std::string& endpoint) const {
        auto it = m_retryAfter.find(endpoint);
        if (it != m_retryAfter.end()) {
            auto now = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                it->second - now).count();
            return diff > 0 ? diff : 0;
        }
        return 0;
    }

    void setGlobalMaxPerSecond(int max) { m_globalMaxPerSecond = max; }
    void setDefaultBurstSize(int burst) { m_defaultBurstSize = burst; }

    int getRemaining(const std::string& endpoint) const {
        auto it = m_limitRemaining.find(endpoint);
        return it != m_limitRemaining.end() ? it->second : -1;
    }

private:
    struct TokenBucket {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        int count = 0;
        int tokens = 20;
        int burstSize = 20;
    };

    std::mutex m_mutex;
    std::map<std::string, TokenBucket> m_windows;
    std::map<std::string, TokenBucket> m_buckets;
    std::map<std::string, std::chrono::steady_clock::time_point> m_retryAfter;
    std::map<std::string, int> m_limitRemaining;
    std::map<std::string, int> m_limitLimit;
    std::map<std::string, int64_t> m_limitResetMs;
    int m_globalMaxPerSecond = 100;
    int m_defaultBurstSize = 20;
};

// ── Retry policy ──

struct RetryPolicy {
    int maxRetries = 3;
    int baseDelayMs = 1000;
    int maxDelayMs = 30000;
    double backoffMultiplier = 2.0;
    double jitterFactor = 0.1;           // Add 0-10% jitter
    std::vector<int> retryableStatusCodes = {429, 500, 502, 503, 504};
    std::vector<std::string> retryableErrors; // error message substrings

    int64_t getDelay(int attempt) const {
        double delay = baseDelayMs * std::pow(backoffMultiplier, attempt);
        // Add jitter
        double jitter = delay * jitterFactor * (rand() / (double)RAND_MAX - 0.5) * 2;
        delay += jitter;
        if (delay > maxDelayMs) delay = maxDelayMs;
        return static_cast<int64_t>(delay);
    }

    bool isRetryable(int statusCode) const {
        for (auto code : retryableStatusCodes) {
            if (code == statusCode) return true;
        }
        return false;
    }

    bool isRetryableError(const std::string& error) const {
        for (const auto& e : retryableErrors) {
            if (error.find(e) != std::string::npos) return true;
        }
        return false;
    }
};

// ── Connection pool config ──

struct ConnectionPoolConfig {
    int maxIdleConnections = 5;
    int keepAliveSeconds = 300;
    int maxConnections = 20;
    int connectTimeoutMs = 10000;
    int readTimeoutMs = 30000;
    int writeTimeoutMs = 30000;
    bool enableKeepAlive = true;
    bool enableCompression = true;
};

// ── HTTP client config ──

struct HttpClientConfig {
    std::string baseUrl;
    std::string userAgent = "PcpMatrix/0.3.0";
    int defaultTimeoutMs = 30000;
    RetryPolicy retryPolicy;
    ConnectionPoolConfig connectionPool;
    std::vector<std::string> allowedCipherSuites;
    bool verifyCertificates = true;
    std::string certificatePin;         // SHA256 pin
    bool enableLogging = false;
    bool enableMetrics = true;
    size_t maxResponseSize = 50 * 1024 * 1024; // 50MB
};

// ── Request interceptor ──

struct HttpRequestInterceptor {
    virtual ~HttpRequestInterceptor() = default;
    virtual void intercept(HttpRequest& request) = 0;
};

// ── Response interceptor ──

struct HttpResponseInterceptor {
    virtual ~HttpResponseInterceptor() = default;
    virtual void intercept(HttpResponse& response) = 0;
};

// ── Auth interceptor ──

class AuthInterceptor : public HttpRequestInterceptor {
public:
    AuthInterceptor() = default;
    void setToken(const std::string& token) { m_token = token; }
    const std::string& token() const { return m_token; }
    bool hasToken() const { return !m_token.empty(); }

    void intercept(HttpRequest& request) override {
        if (!m_token.empty()) {
            request.headers["Authorization"] = "Bearer " + m_token;
        }
    }

private:
    std::string m_token;
};

// ── User agent interceptor ──

class UserAgentInterceptor : public HttpRequestInterceptor {
public:
    explicit UserAgentInterceptor(const std::string& agent) : m_agent(agent) {}

    void intercept(HttpRequest& request) override {
        request.headers["User-Agent"] = m_agent;
    }

private:
    std::string m_agent;
};

// ── Content type interceptor ──

class ContentTypeInterceptor : public HttpRequestInterceptor {
public:
    void intercept(HttpRequest& request) override {
        if (!request.contentType.empty() && request.headers.find("Content-Type") == request.headers.end()) {
            request.headers["Content-Type"] = request.contentType;
        } else if (request.headers.find("Content-Type") == request.headers.end() && !request.body.empty()) {
            request.headers["Content-Type"] = "application/json";
        }
    }
};

// ── Request metrics ──

struct RequestMetrics {
    std::string requestId;
    std::string endpoint;
    HttpMethod method;
    int statusCode = 0;
    int64_t startMs = 0;
    int64_t endMs = 0;
    int64_t latencyMs = 0;
    int retryCount = 0;
    bool success = false;
    std::string error;
    int64_t requestSize = 0;
    int64_t responseSize = 0;
};

using MetricsCallback = std::function<void(const RequestMetrics&)>;

// ── HTTP session ──

class HttpClientSession {
public:
    HttpClientSession() = default;
    virtual ~HttpClientSession() = default;

    virtual HttpResponse execute(const HttpRequest& request) = 0;
    virtual bool isConnected() const = 0;
    virtual void cancel() = 0;
};

// ── Main HTTP client ──

class MatrixHttpClient {
public:
    MatrixHttpClient();
    explicit MatrixHttpClient(const HttpClientConfig& config);
    ~MatrixHttpClient();

    // Configuration
    void setConfig(const HttpClientConfig& config);
    const HttpClientConfig& config() const { return m_config; }

    void setAccessToken(const std::string& token);
    const std::string& accessToken() const { return m_accessToken; }

    void setBaseUrl(const std::string& url);
    const std::string& baseUrl() const { return m_baseUrl; }

    void setUserAgent(const std::string& agent);

    // Interceptors
    void addRequestInterceptor(std::shared_ptr<HttpRequestInterceptor> interceptor);
    void addResponseInterceptor(std::shared_ptr<HttpResponseInterceptor> interceptor);
    void removeAllInterceptors();

    // Rate limiting
    void setGlobalRateLimit(int maxPerSecond);
    void addRateLimitRule(const RateLimitRule& rule);

    // Request execution
    HttpResponse execute(const HttpRequest& request);
    HttpResponse executeWithRetry(const HttpRequest& request);
    void executeAsync(const HttpRequest& request,
                      HttpResponseCallback onSuccess,
                      HttpErrorCallback onError = nullptr);

    // Convenience methods
    HttpResponse get(const std::string& path,
                     const std::map<std::string, std::string>& params = {});
    HttpResponse post(const std::string& path, const json& body);
    HttpResponse put(const std::string& path, const json& body);
    HttpResponse del(const std::string& path);
    HttpResponse patch(const std::string& path, const json& body);

    HttpResponse uploadMedia(const std::string& path,
                              const std::vector<uint8_t>& data,
                              const std::string& filename,
                              const std::string& contentType);
    HttpResponse downloadMedia(const std::string& path,
                                const std::string& localPath,
                                HttpProgressCallback progress = nullptr);

    // JSON convenience
    json getJson(const std::string& path,
                 const std::map<std::string, std::string>& params = {});
    json postJson(const std::string& path, const json& body);
    json putJson(const std::string& path, const json& body);

    // State
    bool isConnected() const;
    bool waitForConnection(int timeoutMs = 5000);

    // Metrics
    std::vector<RequestMetrics> getRecentMetrics();
    double getAverageLatency() const;
    int64_t getTotalRequests() const;
    int64_t getTotalErrors() const;
    void setMetricsCallback(MetricsCallback cb);

    // Connection management
    void warmUp(int connections = 3);
    void evictAll();
    void shutdown();

    // Cookie management
    void setCookie(const std::string& name, const std::string& value);
    void clearCookies();
    std::string getCookie(const std::string& name) const;

    // Proxy
    void setProxy(const std::string& host, int port,
                  const std::string& username = "",
                  const std::string& password = "");

    // Testing/monitoring
    void enableRequestLogging(bool enable);
    void setSimulateLatency(int64_t minMs, int64_t maxMs);

private:
    HttpResponse executeInternal(const HttpRequest& request);
    HttpResponse executeWithRetryInternal(const HttpRequest& request);
    std::string buildFullUrl(const std::string& path,
                              const std::map<std::string, std::string>& params);

    void applyRequestInterceptors(HttpRequest& request);
    void applyResponseInterceptors(HttpResponse& response);
    void parseRateLimitHeaders(const HttpResponse& response, const std::string& endpoint);
    void recordMetrics(const HttpRequest& request, const HttpResponse& response, int retries);

    HttpClientConfig m_config;
    std::string m_baseUrl;
    std::string m_accessToken;
    RateLimitTracker m_rateLimiter;

    std::vector<std::shared_ptr<HttpRequestInterceptor>> m_requestInterceptors;
    std::vector<std::shared_ptr<HttpResponseInterceptor>> m_responseInterceptors;

    // Cookie store
    std::map<std::string, std::string> m_cookies;

    // Metrics
    mutable std::mutex m_metricsMutex;
    std::vector<RequestMetrics> m_recentMetrics;
    MetricsCallback m_metricsCallback;
    int64_t m_totalRequests = 0;
    int64_t m_totalErrors = 0;
    double m_latencySumMs = 0.0;
    int64_t m_latencyCount = 0;

    // Logging
    bool m_requestLogging = false;

    // Testing
    int64_t m_simulateMinLatencyMs = 0;
    int64_t m_simulateMaxLatencyMs = 0;

    // Thread safety
    mutable std::mutex m_mutex;

    // Proxy
    std::string m_proxyHost;
    int m_proxyPort = 0;
    std::string m_proxyUsername;
    std::string m_proxyPassword;
};

} // namespace matrix_sdk
