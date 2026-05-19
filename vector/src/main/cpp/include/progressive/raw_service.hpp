#pragma once

#include <string>
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

} // namespace progressive
