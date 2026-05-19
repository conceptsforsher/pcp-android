#ifndef PROGRESSIVE_OFFLINE_CACHE_HPP
#define PROGRESSIVE_OFFLINE_CACHE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Smart Offline Cache ----
// Computes what to download for offline reading based on:
//   - Available storage space
//   - User-configured room priorities
//   - Media type filters (image/video/audio/file)
//   - Media size limits (max bytes per file)
//   - Room recency and activity
//
// The algorithm budgets available disk space across rooms proportionally
// to their priority, then within each room decides what messages and media
// to download based on recency and user preferences.

// ==== Configuration ====

struct OfflineCacheConfig {
    int64_t availableStorage;      // free disk space (bytes)
    int64_t reservedStorage;       // keep this much free (bytes)
    bool downloadImages = true;    // cache images
    bool downloadVideos = false;   // cache videos (default off — large)
    bool downloadAudio = true;     // cache audio messages
    bool downloadFiles = false;    // cache files (default off)
    int64_t maxFileSize = 50 * 1024 * 1024;  // max single file to download (50MB default)
    int maxMessagesPerRoom = 500;  // how many recent messages to cache per room
    int roomLimit = 20;            // max rooms to cache (top N by priority)
};

// ==== Room Priority ====

struct RoomPriority {
    std::string roomId;
    std::string roomName;
    int priority = 0;              // 0-100, higher = more important. 0 = skip
    int messageCount = 0;          // total messages available
    int mediaFileCount = 0;        // total media files in room
    int64_t estimatedDataSize = 0; // total bytes this room would need (full)
    int64_t lastActivityMs = 0;    // epoch ms — more recent = higher score
    bool isDirect = false;         // DM rooms get priority boost
    bool isFavourite = false;      // favourite rooms get priority boost
    bool isPinned = false;         // pinned to top
};

// ==== Cache Plan ====

struct RoomCachePlan {
    std::string roomId;
    std::string roomName;
    int64_t allocatedBytes = 0;    // budget for this room
    int messagesToDownload = 0;    // how many messages
    int mediaFilesToDownload = 0;  // how many media files
    bool canFitAllMessages = false;
    bool canFitAllMedia = false;
};

struct OfflineCachePlan {
    int64_t totalBudget = 0;       // available for download
    int64_t totalAllocated = 0;    // sum of all room allocations
    int roomsCached = 0;           // how many rooms in plan
    int roomsSkipped = 0;          // rooms with priority=0 or no budget
    std::vector<RoomCachePlan> roomPlans;
    int64_t estimatedTimeMs = 0;   // rough download time estimate
};

// ==== Priority Computation ====

// Compute a room's effective priority score from user settings + recency.
// Original logic: priority base + direct/favourite/pinned bonuses
// + recency bonus (more recent = higher) — age penalty
int computeRoomCacheScore(const RoomPriority& room);

// Sort rooms by cache score (highest first), return top N.
std::vector<RoomPriority> prioritizeRooms(
    std::vector<RoomPriority> rooms,
    int topN = 20);

// ==== Budget Allocation ====

// Allocate available storage across prioritized rooms.
// Budget is proportional to priority score.
// Rooms with priority=0 get nothing.
OfflineCachePlan allocateCacheBudget(
    const std::vector<RoomPriority>& rooms,
    const OfflineCacheConfig& config);

// ==== Media Filtering ====

// Check if a media file should be downloaded based on config.
// @param mimeType  e.g. "image/png", "video/mp4"
// @param fileSize  bytes
// @return true if this file should be cached
bool shouldCacheMedia(const OfflineCacheConfig& config,
    const std::string& mimeType, int64_t fileSize);

// ==== Storage Tracking ====

// Estimate cache size for a set of messages.
int64_t estimateMessageCacheSize(int messageCount, int avgBodySize = 500);

// Estimate total media size for a room.
int64_t estimateMediaCacheSize(int mediaCount,
    int64_t avgMediaSize = 500 * 1024);  // 500KB default

// Check if there's enough space for the planned download.
bool canFitInStorage(int64_t required, int64_t available, int64_t reserved);

// ==== JSON Serialization ====

std::string offlineCachePlanToJson(const OfflineCachePlan& plan);
std::string roomPriorityToJson(const RoomPriority& room);

// ================================================================
// Offline Cache Manager — cache state, eviction, statistics
// Extends the existing budget allocator with full lifecycle mgmt.
// ================================================================

// ---- Cache State (per room) ----

struct RoomCacheState {
    std::string roomId;
    int64_t cachedBytes = 0;         // Current cached size
    int cachedMessages = 0;          // Current cached message count
    int cachedMedia = 0;             // Current cached media count
    int64_t lastAccessMs = 0;        // When cache was last used
    int64_t createdAtMs = 0;         // When cache was created
    int priority = 0;                // Priority score
    bool isComplete = false;         // Full cache completed
    int64_t expireAfterMs = 0;       // Auto-expire (0 = never)
};

// ---- Cache Statistics ----

struct OLCacheStats {
    int64_t totalCachedBytes = 0;    // Total data cached
    int totalCachedMessages = 0;
    int totalCachedMedia = 0;
    int roomsCached = 0;
    int cacheHits = 0;               // Read from cache
    int cacheMisses = 0;             // Had to fetch from network
    int64_t bandwidthSavedBytes = 0; // Estimated bandwidth saved
    int evictions = 0;               // Items removed to free space
    int64_t evictedBytes = 0;
    int64_t oldestCacheMs = 0;       // Oldest cached entry age
    int64_t newestCacheMs = 0;
    double hitRate() const {
        int total = cacheHits + cacheMisses;
        return total > 0 ? static_cast<double>(cacheHits) / total * 100.0 : 0.0;
    }
};

// ---- Cache Pressure Level ----

enum class CachePressure {
    NONE = 0,            // Plenty of space
    LOW = 1,             // < 75% full — monitor
    MEDIUM = 2,          // < 50% free — start evicting
    HIGH = 3,            // < 25% free — aggressive eviction
    CRITICAL = 4,        // < 10 MB free — emergency
};

// ---- Offline Cache Manager ----

class OfflineCacheManager {
public:
    OfflineCacheManager();

    // ====== Config ======

    void setConfig(const OfflineCacheConfig& config);
    OfflineCacheConfig getConfig() const;

    // ====== Room Registration ======
    // Register room priority for cache budgeting.

    void registerRoom(const RoomPriority& room);
    void unregisterRoom(const std::string& roomId);
    void updateRoomActivity(const std::string& roomId, int64_t timestampMs);

    // ====== Cache State ======

    // Record that data was cached for a room.
    void recordCached(const std::string& roomId, int64_t bytes, int messages, int media);

    // Record a cache hit (read from cache).
    void recordHit(const std::string& roomId, int64_t bytes);

    // Record a cache miss (had to fetch).
    void recordMiss(const std::string& roomId, int64_t bytes);

    // Get current cache state for a room.
    bool getRoomState(const std::string& roomId, RoomCacheState& out) const;

    // ====== Eviction ======

    // Check storage pressure level.
    CachePressure getPressure(int64_t availableStorage, int64_t reservedStorage) const;

    // Evict rooms to free targetBytes of space.
    // Evicts lowest priority + oldest access first.
    // Returns list of evicted room IDs.
    std::vector<std::string> evictToFree(int64_t targetBytes, int64_t availableStorage, int64_t reservedStorage);

    // Evict a specific room.
    int64_t evictRoom(const std::string& roomId);

    // Evict expired caches.
    std::vector<std::string> evictExpired();

    // ====== Statistics ======

    OLCacheStats getStats() const;
    void resetStats();

    // ====== Cache Plan ======

    // Generate a full cache plan using the existing allocator.
    OfflineCachePlan generatePlan();

    // ====== Serialization ======

    std::string statsToJson() const;
    std::string roomStateToJson(const RoomCacheState& state) const;
    std::string pressureToJson(CachePressure pressure) const;

private:
    OfflineCacheConfig config_;
    std::vector<RoomPriority> rooms_;
    std::unordered_map<std::string, RoomCacheState> cacheState_; // roomId → cache state
    OLCacheStats stats_;

    RoomCacheState& getOrCreateState(const std::string& roomId);
};

} // namespace progressive

#endif // PROGRESSIVE_OFFLINE_CACHE_HPP
