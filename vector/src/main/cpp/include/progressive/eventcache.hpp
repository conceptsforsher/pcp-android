#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <chrono>

namespace progressive {

// ================================================================
// EventCache Policy & Configuration
// ================================================================

// Original Kotlin: EventCachePolicy enum
enum class EventCachePolicy {
    LRU,
    FIFO,
    TTL
};

// Original Kotlin: CacheEntryMetadata
struct CacheEntryMetadata {
    int64_t addedAt = 0;
    int64_t lastAccessedAt = 0;
    int accessCount = 0;
    int64_t size = 0;
};

// Original Kotlin: EventCacheConfig
struct EventCacheConfig {
    size_t maxSize = 1000;
    EventCachePolicy policy = EventCachePolicy::LRU;
    int64_t ttlMs = 3600000;
    bool evictOnLowMemory = true;
};

// Original Kotlin: EventCacheStats
struct EventCacheStats {
    int64_t hits = 0;
    int64_t misses = 0;
    int64_t evictions = 0;
    size_t size = 0;
    double hitRate = 0.0;
};

struct CachedEvent {
    std::string eventId;
    std::string senderId;
    std::string senderName;
    std::string timestamp;
    std::string body;
    std::string msgType;
    std::string eventType;
    std::string relationType;
    std::string sourceEventId;
    std::string formattedBody;
    bool isEncrypted = false;
    bool sentByMe = false;
    bool hasFailed = false;
    int reactionCount = 0;
    bool hasBeenEdited = false;
};

class EventCache {
public:
    // Insert or update a cached event. Called when timeline loads events.
    void put(const CachedEvent& event);

    // Get a single event by ID. Returns nullptr if not cached.
    const CachedEvent* get(const std::string& eventId) const;

    // Get all events that react to / relate to a given source event.
    std::vector<const CachedEvent*> getRelations(const std::string& sourceEventId) const;

    // Get quick context menu data for a given eventId.
    // Returns JSON string with all fields needed for Stage 2 context menu.
    std::string getContextData(const std::string& eventId) const;

    // Clear all cached data.
    void clear();

    // Number of cached events.
    size_t size() const { return events_.size(); }

    // ================================================================
    // Extended Cache Management
    // ================================================================

    // Configure the cache policy and limits.
    // Original Kotlin: configure()
    void configure(const EventCacheConfig& config);

    // Get the current cache configuration.
    // Original Kotlin: getConfig()
    const EventCacheConfig& getConfig() const;

    // Get cache statistics (hits, misses, evictions, hit rate).
    // Original Kotlin: getCacheStats()
    EventCacheStats getCacheStats() const;

    // Evict one or more entries based on the configured policy.
    // Original Kotlin: evictByPolicy()
    void evictByPolicy();

    // Check if eviction is needed (size >= maxSize).
    // Original Kotlin: isEvictionNeeded()
    bool isEvictionNeeded() const;

    // Preload a batch of events into the cache.
    // Original Kotlin: warmCache()
    void warmCache(const std::vector<CachedEvent>& events);

    // Clear all events belonging to a specific room.
    // Original Kotlin: invalidateRoom()
    void invalidateRoom(const std::string& roomId);

    // Clear all events sent by a specific sender.
    // Original Kotlin: invalidateSender()
    void invalidateSender(const std::string& senderId);

    // Get the current cache hit rate (0.0 to 1.0).
    // Original Kotlin: getCacheHitRate()
    double getCacheHitRate() const;

private:
    std::unordered_map<std::string, CachedEvent> events_;
    // Reverse index: source event → events that relate to it
    std::unordered_map<std::string, std::vector<std::string>> relationIndex_;

    // Cache management
    EventCacheConfig config_;
    mutable int64_t hits_ = 0;
    mutable int64_t misses_ = 0;
    int64_t evictions_ = 0;
    std::unordered_map<std::string, CacheEntryMetadata> metadata_;
};

} // namespace progressive
