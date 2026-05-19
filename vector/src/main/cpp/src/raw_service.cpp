#include "progressive/raw_service.hpp"
#include "progressive/login_utils.hpp"

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

} // namespace progressive
