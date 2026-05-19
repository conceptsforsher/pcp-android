#ifndef PROGRESSIVE_CACHE_MANAGER_HPP
#define PROGRESSIVE_CACHE_MANAGER_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// Cache strategy — from CacheStrategy.kt (35L)
enum class CacheStrategy { NoCache, TtlCache, InfiniteCache };

struct CacheEntry {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string body;          // message preview
    int64_t timestamp = 0;     // epoch ms
    int64_t sizeBytes = 0;     // estimated cache size
    std::string msgType;
};

struct CacheStats {
    int entryCount = 0;
    int roomCount = 0;
    int64_t totalBytes = 0;
    std::string oldestEntryDate;
    std::string newestEntryDate;
};

class CacheManager {
public:
    // Add/update a cache entry.
    void track(const std::string& eventId, const std::string& roomId,
               const std::string& roomName, int64_t timestamp, int64_t sizeBytes,
               const std::string& msgType, const std::string& body);

    // Get stats about the cache.
    CacheStats getStats() const;

    // Get entries for a specific room.
    std::vector<CacheEntry> getByRoom(const std::string& roomId) const;

    // Get entries older than a timestamp.
    std::vector<CacheEntry> getOlderThan(int64_t beforeTs) const;

    // Get entries between two timestamps.
    std::vector<CacheEntry> getByDateRange(int64_t fromTs, int64_t toTs) const;

    // Mark entries for deletion (returns list of eventIds).
    std::vector<std::string> markForDeletion(const std::vector<std::string>& eventIds);

    // Get all tracked entries.
    const std::vector<CacheEntry>& entries() const { return entries_; }

    // Clear tracking data.
    void clear();

    // Get unique room IDs with cached data.
    std::vector<std::string> getCachedRoomIds() const;

    // Format cache stats as JSON.
    std::string statsToJson() const;

    // Format entry list as JSON.
    static std::string entriesToJson(const std::vector<CacheEntry>& entries);

private:
    std::vector<CacheEntry> entries_;
};

} // namespace progressive

#endif // PROGRESSIVE_CACHE_MANAGER_HPP
