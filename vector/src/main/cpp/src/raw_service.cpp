#include "progressive/raw_service.hpp"
#include "progressive/login_utils.hpp"
#include <algorithm>
#include <cstdio>

namespace progressive {

// ==== JSON Escape/Unescape Helpers ====

static std::string escapeJsonString(const std::string& s) {
    std::string result = "\"";
    for (char c : s) {
        if (c == '"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else if (c == '\n') result += "\\n";
        else if (c == '\r') result += "\\r";
        else if (c == '\t') result += "\\t";
        else result += c;
    }
    result += "\"";
    return result;
}

static std::string unescapeJsonString(const std::string& s) {
    std::string result;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            char next = s[i + 1];
            if (next == '"') { result += '"'; i++; }
            else if (next == '\\') { result += '\\'; i++; }
            else if (next == 'n') { result += '\n'; i++; }
            else if (next == 'r') { result += '\r'; i++; }
            else if (next == 't') { result += '\t'; i++; }
            else { result += s[i]; }
        } else {
            result += s[i];
        }
    }
    return result;
}

static std::string extractJsonStringForCache(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size()) {
        if (json[end] == '\\') { end += 2; continue; }
        if (json[end] == '"') break;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64ForCache(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

// ==== Cache Freshness Check ====
//
// Original Kotlin (GetUrlTask.kt:76-79):
//   var isCacheValid = false
//   monarchy.doWithRealm { realm ->
//       val entity = RawCacheEntity.get(realm, url)
//       dataFromCache = entity?.data
//       isCacheValid = entity != null &&
//           Date().time < entity.lastUpdatedTimestamp + validityDurationInMillis
//   }
//
// Original Kotlin (GetUrlTask.kt:52-67):
//   when (params.cacheStrategy) {
//       NoCache -> doRequest
//       TtlCache -> doRequestWithCache(validityDurationInMillis, strict)
//       InfiniteCache -> doRequestWithCache(Long.MAX_VALUE, true)
//   }

bool shouldFetchFromNetwork(
    const CacheStrategy& strategy,
    const RawCacheEntry& cachedEntry,
    int64_t nowMillis)
{
    switch (strategy.type) {
        case CacheStrategyType::NO_CACHE:
            // Original Kotlin: CacheStrategy.NoCache -> always fetch
            return true;

        case CacheStrategyType::TTL_CACHE:
            // Original Kotlin: check TTL
            if (!cachedEntry.isValid) return true;
            if (cachedEntry.isFresh(strategy.validityDurationMillis, nowMillis))
                return false;
            // Cache expired — fetch unless we can use stale data
            return true;

        case CacheStrategyType::INFINITE_CACHE:
            // Original Kotlin: CacheStrategy.InfiniteCache -> use cache if valid
            if (cachedEntry.isValid) return false;
            return true;
    }
    return true;
}

// ==== Cache Serialization ====
//
// Original Kotlin (RawCacheEntity):
//   stored in Realm database, fields: url, data, lastUpdatedTimestamp

std::string rawCacheEntryToJson(const RawCacheEntry& entry) {
    std::string json = "{";
    json += "\"url\":" + escapeJsonString(entry.url) + ",";
    json += "\"data\":" + escapeJsonString(entry.data) + ",";
    json += "\"lastUpdatedTimestamp\":" + std::to_string(entry.lastUpdatedTimestamp);
    json += "}";
    return json;
}

// ==== Cache Key ====

// Simple deterministic key from URL for cache indexing.

std::string cacheKeyForUrl(const std::string& url) {
    // Simple hash: use the URL itself as key (encoding-safe)
    // For longer URLs, use a truncated hash
    return url;
}

// ================================================================
// HttpMethod
// ================================================================

const char* httpMethodToString(HttpMethod method) {
    switch (method) {
        case HttpMethod::GET:     return "GET";
        case HttpMethod::POST:    return "POST";
        case HttpMethod::PUT:     return "PUT";
        case HttpMethod::DELETE_: return "DELETE";
        case HttpMethod::PATCH:   return "PATCH";
        case HttpMethod::HEAD:    return "HEAD";
    }
    return "GET";
}

// ================================================================
// RawRequest / RawResponse
// ================================================================

RawRequest buildRawRequest(HttpMethod method, const std::string& url,
    const std::unordered_map<std::string, std::string>& headers,
    const std::string& body, int timeoutMs)
{
    RawRequest req;
    req.method = method;
    req.url = url;
    req.headers = headers;
    req.body = body;
    req.timeoutMs = timeoutMs;
    return req;
}

// Simple percent-encode for query string values.
static std::string percentEncode(const std::string& s) {
    std::string result;
    for (unsigned char c : s) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' ||
            c == '.' || c == '~') {
            result += c;
        } else {
            char hex[4];
            snprintf(hex, sizeof(hex), "%%%02X", c);
            result += hex;
        }
    }
    return result;
}

// Parse raw HTTP response text into RawResponse.
// Expects: "HTTP/1.1 200 OK\r\nHeader: value\r\n\r\nbody"
RawResponse parseRawResponse(const std::string& rawHttpResponse) {
    RawResponse resp;

    size_t headerEnd = rawHttpResponse.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        headerEnd = rawHttpResponse.find("\n\n");
        if (headerEnd == std::string::npos) return resp;
        headerEnd += 2; // past \n\n
    } else {
        headerEnd += 4; // past \r\n\r\n
    }

    // Parse status line: "HTTP/1.1 200 OK"
    auto statusEnd = rawHttpResponse.find("\r\n");
    if (statusEnd == std::string::npos)
        statusEnd = rawHttpResponse.find('\n');
    std::string statusLine = rawHttpResponse.substr(0, statusEnd);

    // Extract status code
    auto codeStart = statusLine.find(' ');
    if (codeStart != std::string::npos) {
        codeStart++;
        auto codeEnd = statusLine.find(' ', codeStart);
        std::string codeStr = statusLine.substr(codeStart, codeEnd - codeStart);
        resp.statusCode = std::stoi(codeStr);
        if (codeEnd != std::string::npos) {
            resp.statusMessage = statusLine.substr(codeEnd + 1);
        }
    }

    // Parse headers
    size_t pos = statusEnd + 2; // skip \r\n
    while (pos < headerEnd && rawHttpResponse[pos] != '\r' && rawHttpResponse[pos] != '\n') {
        size_t lineEnd = rawHttpResponse.find("\r\n", pos);
        if (lineEnd == std::string::npos || lineEnd > headerEnd)
            lineEnd = rawHttpResponse.find('\n', pos);
        if (lineEnd == std::string::npos || lineEnd > headerEnd)
            break;

        std::string line = rawHttpResponse.substr(pos, lineEnd - pos);
        auto colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 1);
            // Trim val
            while (!val.empty() && (val.front() == ' ' || val.front() == '\t'))
                val.erase(0, 1);
            while (!val.empty() && (val.back() == ' ' || val.back() == '\t' ||
                                     val.back() == '\r'))
                val.pop_back();
            resp.headers[key] = val;
        }

        pos = lineEnd + 2; // skip \r\n
        if (pos >= rawHttpResponse.size()) break;
    }

    // Body starts after headers
    if (headerEnd < rawHttpResponse.size()) {
        resp.body = rawHttpResponse.substr(headerEnd);
    }

    return resp;
}

void addAuthHeader(std::unordered_map<std::string, std::string>& headers,
    const std::string& token)
{
    headers["Authorization"] = "Bearer " + token;
}

void addContentTypeHeader(std::unordered_map<std::string, std::string>& headers,
    const std::string& contentType)
{
    headers["Content-Type"] = contentType;
}

std::string buildQueryString(
    const std::unordered_map<std::string, std::string>& params)
{
    if (params.empty()) return "";

    std::string result = "?";
    bool first = true;
    for (const auto& [key, val] : params) {
        if (!first) result += "&";
        first = false;
        result += percentEncode(key) + "=" + percentEncode(val);
    }
    return result;
}

std::string buildFormBody(
    const std::unordered_map<std::string, std::string>& params)
{
    std::string result;
    bool first = true;
    for (const auto& [key, val] : params) {
        if (!first) result += "&";
        first = false;
        result += percentEncode(key) + "=" + percentEncode(val);
    }
    return result;
}

std::string buildMultipartBody(
    const std::vector<MultipartPart>& parts,
    const std::string& boundary)
{
    std::string bd = boundary;
    if (bd.empty()) {
        bd = "----ProgressiveMultipartBoundary";
    }

    std::string result;
    for (const auto& part : parts) {
        result += "--" + bd + "\r\n";
        result += "Content-Disposition: form-data; name=\"" + part.name + "\"";
        if (!part.filename.empty()) {
            result += "; filename=\"" + part.filename + "\"";
        }
        result += "\r\n";
        if (!part.contentType.empty()) {
            result += "Content-Type: " + part.contentType + "\r\n";
        }
        result += "\r\n";
        result += part.body;
        result += "\r\n";
    }
    result += "--" + bd + "--\r\n";
    return result;
}

// ================================================================
// Retry Policy
// ================================================================

bool applyRetryPolicy(const RetryPolicy& policy, int statusCode, int retryCount) {
    if (retryCount >= policy.maxRetries) return false;
    if (policy.retryableStatusCodes.empty()) {
        // Default: retry on 429 and 5xx
        return statusCode == 429 || (statusCode >= 500 && statusCode < 600);
    }
    for (int c : policy.retryableStatusCodes) {
        if (c == statusCode) return true;
    }
    return false;
}

int calcRetryDelay(const RetryPolicy& policy, int retryCount) {
    if (retryCount <= 0) return 0;

    int delay = policy.baseDelayMs;
    switch (policy.backoffStrategy) {
        case BackoffStrategy::NONE:
            return 0;
        case BackoffStrategy::FIXED:
            return policy.baseDelayMs;
        case BackoffStrategy::LINEAR:
            delay = policy.baseDelayMs * retryCount;
            break;
        case BackoffStrategy::EXPONENTIAL: {
            int factor = 1;
            for (int i = 0; i < retryCount; i++) factor *= 2;
            delay = policy.baseDelayMs * factor;
            break;
        }
    }
    if (delay > policy.maxDelayMs) delay = policy.maxDelayMs;
    return delay;
}

// ================================================================
// Request Logger
// ================================================================

void RequestLogger::addLog(const RequestLog& log) {
    if (logs_.size() >= MAX_LOG_ENTRIES) {
        logs_.pop_front();
    }
    logs_.push_back(log);
}

std::vector<RequestLog> RequestLogger::getLogs() const {
    return {logs_.begin(), logs_.end()};
}

std::vector<RequestLog> RequestLogger::getRecentLogs(size_t count) const {
    if (count >= logs_.size()) return {logs_.begin(), logs_.end()};
    return {logs_.end() - static_cast<ptrdiff_t>(count), logs_.end()};
}

void RequestLogger::clear() {
    logs_.clear();
}

size_t RequestLogger::size() const {
    return logs_.size();
}

// ================================================================
// Request Metrics
// ================================================================

RequestMetrics computeRequestMetrics(const std::vector<RequestLog>& logs) {
    RequestMetrics m;
    if (logs.empty()) return m;

    std::vector<double> latencies;
    latencies.reserve(logs.size());

    for (const auto& log : logs) {
        m.totalRequests++;
        if (log.response.isOk()) {
            m.successfulRequests++;
        } else {
            m.failedRequests++;
        }
        latencies.push_back(static_cast<double>(log.durationMs));
    }

    // Average
    double sum = 0.0;
    for (double l : latencies) sum += l;
    m.avgLatencyMs = sum / latencies.size();

    // Percentiles
    std::sort(latencies.begin(), latencies.end());
    size_t n = latencies.size();
    auto pct = [&](double p) -> double {
        size_t idx = static_cast<size_t>(n * p / 100.0);
        if (idx >= n) idx = n - 1;
        return latencies[idx];
    };

    m.p50LatencyMs = pct(50.0);
    m.p95LatencyMs = pct(95.0);
    m.p99LatencyMs = pct(99.0);

    if (!logs.empty()) {
        m.firstRequestTs = logs.front().timestampMs;
        m.lastRequestTs = logs.back().timestampMs;
    }

    return m;
}

} // namespace progressive
