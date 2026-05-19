#include "progressive/offline_cache.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <ctime>

namespace progressive {

// ==== Priority Computation ====

int computeRoomCacheScore(const RoomPriority& room) {
    int score = room.priority;

    // Original logic: priority base (0-100) + bonuses
    if (room.isFavourite) score += 30;
    if (room.isDirect) score += 20;
    if (room.isPinned) score += 40;

    // Recency bonus: rooms active in last 24h get up to +20
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    auto ageHours = (now - room.lastActivityMs) / (1000 * 60 * 60);
    if (ageHours < 1) score += 20;
    else if (ageHours < 6) score += 15;
    else if (ageHours < 24) score += 10;
    else if (ageHours < 72) score += 5;
    else if (ageHours > 168) score -= 10; // > 1 week — penalty

    // Message/media bonus: rooms with more content get slight priority
    if (room.messageCount > 1000) score += 5;
    if (room.mediaFileCount > 100) score += 3;

    return std::max(0, std::min(score, 200)); // clamp 0-200
}

std::vector<RoomPriority> prioritizeRooms(
    std::vector<RoomPriority> rooms, int topN)
{
    // Sort by score descending, filter out priority=0
    std::sort(rooms.begin(), rooms.end(),
        [](const RoomPriority& a, const RoomPriority& b) {
            return computeRoomCacheScore(a) > computeRoomCacheScore(b);
        });

    // Remove rooms with score=0
    rooms.erase(
        std::remove_if(rooms.begin(), rooms.end(),
            [](const RoomPriority& r) { return computeRoomCacheScore(r) <= 0; }),
        rooms.end());

    if (static_cast<int>(rooms.size()) > topN) {
        rooms.resize(topN);
    }

    return rooms;
}

// ==== Budget Allocation ====

OfflineCachePlan allocateCacheBudget(
    const std::vector<RoomPriority>& rooms,
    const OfflineCacheConfig& config)
{
    OfflineCachePlan plan;

    // Available budget
    int64_t usable = config.availableStorage - config.reservedStorage;
    if (usable <= 0) return plan;
    plan.totalBudget = usable;

    auto prioritized = prioritizeRooms(
        const_cast<std::vector<RoomPriority>&>(rooms), config.roomLimit);

    // Calculate total priority weight
    int totalWeight = 0;
    for (const auto& r : prioritized) {
        totalWeight += computeRoomCacheScore(r);
    }
    if (totalWeight == 0) return plan;

    // Allocate proportional to priority score
    for (const auto& room : prioritized) {
        RoomCachePlan rp;
        rp.roomId = room.roomId;
        rp.roomName = room.roomName;

        // Proportional budget
        int weight = computeRoomCacheScore(room);
        rp.allocatedBytes = usable * weight / totalWeight;

        // How many messages can we cache with this budget?
        int64_t msgMem = estimateMessageCacheSize(std::min(
            room.messageCount, config.maxMessagesPerRoom));
        if (msgMem <= rp.allocatedBytes) {
            rp.messagesToDownload = std::min(room.messageCount, config.maxMessagesPerRoom);
            rp.canFitAllMessages = true;
        } else {
            rp.messagesToDownload = static_cast<int>(
                rp.allocatedBytes / estimateMessageCacheSize(1));
            rp.canFitAllMessages = false;
        }

        // How many media files can we cache?
        int64_t medMem = estimateMediaCacheSize(room.mediaFileCount);
        if (medMem > 0 && rp.allocatedBytes > msgMem) {
            int64_t mediaBudget = rp.allocatedBytes - msgMem;
            if (mediaBudget >= medMem) {
                rp.mediaFilesToDownload = room.mediaFileCount;
                rp.canFitAllMedia = true;
            } else {
                rp.mediaFilesToDownload = static_cast<int>(
                    mediaBudget / estimateMediaCacheSize(1));
                rp.canFitAllMedia = false;
            }
        }

        plan.totalAllocated += rp.allocatedBytes;
        plan.roomPlans.push_back(rp);
        plan.roomsCached++;
    }

    // Estimate download time (rough: 1MB/s on average mobile)
    plan.estimatedTimeMs = plan.totalAllocated / 1024; // ms per KB

    return plan;
}

// ==== Media Filtering ====

bool shouldCacheMedia(const OfflineCacheConfig& config,
    const std::string& mimeType, int64_t fileSize)
{
    // Size limit
    if (fileSize > config.maxFileSize) return false;

    // Type filter
    if (mimeType.find("image/") == 0) return config.downloadImages;
    if (mimeType.find("video/") == 0) return config.downloadVideos;
    if (mimeType.find("audio/") == 0) return config.downloadAudio;
    if (mimeType.find("application/") == 0 || mimeType.find("text/") == 0)
        return config.downloadFiles;

    return false; // Unknown type — skip
}

// ==== Size Estimation ====

int64_t estimateMessageCacheSize(int messageCount, int avgBodySize) {
    // Rough: message body + metadata ~ avgBodySize + 200 bytes overhead
    return static_cast<int64_t>(messageCount) * (avgBodySize + 200);
}

int64_t estimateMediaCacheSize(int mediaCount, int64_t avgMediaSize) {
    return static_cast<int64_t>(mediaCount) * avgMediaSize;
}

bool canFitInStorage(int64_t required, int64_t available, int64_t reserved) {
    return (available - reserved) >= required;
}

// ==== JSON ====

std::string roomPriorityToJson(const RoomPriority& room) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(room.roomId) << R"(",)";
    json << R"("priority": )" << room.priority << ",";
    json << R"("score": )" << computeRoomCacheScore(room) << ",";
    json << R"("messageCount": )" << room.messageCount << ",";
    json << R"("mediaFileCount": )" << room.mediaFileCount << "}";
    return json.str();
}

std::string offlineCachePlanToJson(const OfflineCachePlan& plan) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"totalBudget": )" << plan.totalBudget << ",";
    json << R"("totalAllocated": )" << plan.totalAllocated << ",";
    json << R"("roomsCached": )" << plan.roomsCached << ",";
    json << R"("roomsSkipped": )" << plan.roomsSkipped << ",";
    json << R"("estimatedTimeMs": )" << plan.estimatedTimeMs << ",";
    json << R"("roomPlans": [)";
    for (size_t i = 0; i < plan.roomPlans.size(); ++i) {
        if (i > 0) json << ",";
        const auto& rp = plan.roomPlans[i];
        json << R"({"roomId": ")" << esc(rp.roomId) << R"(",)";
        json << R"("allocatedBytes": )" << rp.allocatedBytes << ",";
        json << R"("messagesToDownload": )" << rp.messagesToDownload << ",";
        json << R"("mediaFilesToDownload": )" << rp.mediaFilesToDownload << ",";
        json << R"("canFitAllMessages": )" << (rp.canFitAllMessages ? "true" : "false");
        json << "}";
    }
    json << "]}";
    return json.str();
}

// ================================================================
// OfflineCacheManager Implementation
// ================================================================

OfflineCacheManager::OfflineCacheManager() {}

void OfflineCacheManager::setConfig(const OfflineCacheConfig& config) { config_ = config; }
OfflineCacheConfig OfflineCacheManager::getConfig() const { return config_; }

// ====== Room Registration ======

void OfflineCacheManager::registerRoom(const RoomPriority& room) {
    for (auto& r : rooms_) {
        if (r.roomId == room.roomId) {
            r = room;
            return;
        }
    }
    rooms_.push_back(room);
    // Initialize cache state
    RoomCacheState state;
    state.roomId = room.roomId;
    state.priority = computeRoomCacheScore(room);
    state.createdAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    if (cacheState_.find(room.roomId) == cacheState_.end()) {
        cacheState_[room.roomId] = state;
    }
}

void OfflineCacheManager::unregisterRoom(const std::string& roomId) {
    rooms_.erase(std::remove_if(rooms_.begin(), rooms_.end(),
        [&](const RoomPriority& r) { return r.roomId == roomId; }), rooms_.end());
    cacheState_.erase(roomId);
}

void OfflineCacheManager::updateRoomActivity(const std::string& roomId, int64_t timestampMs) {
    auto it = cacheState_.find(roomId);
    if (it != cacheState_.end()) it->second.lastAccessMs = timestampMs;
    for (auto& r : rooms_) {
        if (r.roomId == roomId) r.lastActivityMs = timestampMs;
    }
}

// ====== Cache State ======

RoomCacheState& OfflineCacheManager::getOrCreateState(const std::string& roomId) {
    auto it = cacheState_.find(roomId);
    if (it != cacheState_.end()) return it->second;
    RoomCacheState state;
    state.roomId = roomId;
    state.createdAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    cacheState_[roomId] = state;
    return cacheState_[roomId];
}

void OfflineCacheManager::recordCached(const std::string& roomId, int64_t bytes, int messages, int media) {
    auto& state = getOrCreateState(roomId);
    state.cachedBytes += bytes;
    state.cachedMessages += messages;
    state.cachedMedia += media;
    state.lastAccessMs = static_cast<int64_t>(std::time(nullptr)) * 1000;

    stats_.totalCachedBytes += bytes;
    stats_.totalCachedMessages += messages;
    stats_.totalCachedMedia += media;
    if (stats_.oldestCacheMs == 0) stats_.oldestCacheMs = state.createdAtMs;
    stats_.newestCacheMs = state.lastAccessMs;
}

void OfflineCacheManager::recordHit(const std::string& roomId, int64_t bytes) {
    auto& state = getOrCreateState(roomId);
    state.lastAccessMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    stats_.cacheHits++;
    stats_.bandwidthSavedBytes += bytes;
}

void OfflineCacheManager::recordMiss(const std::string& roomId, int64_t bytes) {
    (void)bytes;
    stats_.cacheMisses++;
}

bool OfflineCacheManager::getRoomState(const std::string& roomId, RoomCacheState& out) const {
    auto it = cacheState_.find(roomId);
    if (it == cacheState_.end()) return false;
    out = it->second;
    return true;
}

// ====== Eviction ======

CachePressure OfflineCacheManager::getPressure(int64_t availableStorage, int64_t reservedStorage) const {
    int64_t freeSpace = availableStorage - reservedStorage;
    int64_t usedCache = stats_.totalCachedBytes;

    if (freeSpace <= 10 * 1024 * 1024) return CachePressure::CRITICAL; // < 10 MB
    if (freeSpace <= availableStorage / 4) return CachePressure::HIGH;    // < 25%
    if (freeSpace <= availableStorage / 2) return CachePressure::MEDIUM;  // < 50%
    if (usedCache > availableStorage * 3 / 4) return CachePressure::LOW;  // cache > 75%
    return CachePressure::NONE;
}

std::vector<std::string> OfflineCacheManager::evictToFree(int64_t targetBytes, int64_t availableStorage, int64_t reservedStorage) {
    std::vector<std::string> evicted;
    if (targetBytes <= 0) return evicted;

    int64_t freeSpace = availableStorage - reservedStorage - stats_.totalCachedBytes;
    int64_t needToFree = targetBytes - freeSpace;
    if (needToFree <= 0) return evicted;

    // Sort rooms by: lowest priority first, then oldest access
    std::vector<std::pair<std::string, RoomCacheState>> sorted;
    for (const auto& [rid, state] : cacheState_) sorted.push_back({rid, state});

    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) {
            const auto& sa = a.second;
            const auto& sb = b.second;
            // Evict expired first
            bool aExpired = sa.expireAfterMs > 0 &&
                (static_cast<int64_t>(std::time(nullptr)) * 1000 - sa.lastAccessMs) > sa.expireAfterMs;
            bool bExpired = sb.expireAfterMs > 0 &&
                (static_cast<int64_t>(std::time(nullptr)) * 1000 - sb.lastAccessMs) > sb.expireAfterMs;
            if (aExpired != bExpired) return aExpired;
            if (sa.priority != sb.priority) return sa.priority < sb.priority;
            return sa.lastAccessMs < sb.lastAccessMs;
        });

    int64_t freed = 0;
    for (const auto& [rid, state] : sorted) {
        if (freed >= needToFree) break;
        freed += evictRoom(rid);
        evicted.push_back(rid);
    }

    return evicted;
}

int64_t OfflineCacheManager::evictRoom(const std::string& roomId) {
    auto it = cacheState_.find(roomId);
    if (it == cacheState_.end()) return 0;

    int64_t freed = it->second.cachedBytes;

    stats_.totalCachedBytes -= freed;
    stats_.totalCachedMessages -= it->second.cachedMessages;
    stats_.totalCachedMedia -= it->second.cachedMedia;
    stats_.evictions++;
    stats_.evictedBytes += freed;

    cacheState_.erase(it);

    // Keep room priority but reset cache state
    RoomCacheState empty;
    empty.roomId = roomId;
    empty.createdAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    cacheState_[roomId] = empty;

    stats_.roomsCached = static_cast<int>(cacheState_.size());
    return freed;
}

std::vector<std::string> OfflineCacheManager::evictExpired() {
    std::vector<std::string> evicted;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;

    for (const auto& [rid, state] : cacheState_) {
        if (state.expireAfterMs > 0 &&
            state.lastAccessMs > 0 &&
            (now - state.lastAccessMs) > state.expireAfterMs) {
            evicted.push_back(rid);
        }
    }

    for (const auto& rid : evicted) evictRoom(rid);
    return evicted;
}

// ====== Statistics ======

OLCacheStats OfflineCacheManager::getStats() const {
    auto s = stats_;
    s.roomsCached = static_cast<int>(cacheState_.size());
    return s;
}

void OfflineCacheManager::resetStats() {
    stats_ = OLCacheStats{};
}

// ====== Cache Plan ======

OfflineCachePlan OfflineCacheManager::generatePlan() {
    return allocateCacheBudget(rooms_, config_);
}

// ====== Serialization ======

std::string OfflineCacheManager::statsToJson() const {
    auto s = stats_;
    s.roomsCached = static_cast<int>(cacheState_.size());

    std::ostringstream os;
    os << R"({"total_cached_mb":)" << (s.totalCachedBytes / 1048576.0)
       << R"(,"total_messages":)" << s.totalCachedMessages
       << R"(,"total_media":)" << s.totalCachedMedia
       << R"(,"rooms_cached":)" << s.roomsCached
       << R"(,"hits":)" << s.cacheHits
       << R"(,"misses":)" << s.cacheMisses
       << R"(,"hit_rate":)" << s.hitRate()
       << R"(,"bandwidth_saved_mb":)" << (s.bandwidthSavedBytes / 1048576.0)
       << R"(,"evictions":)" << s.evictions
       << R"(,"evicted_mb":)" << (s.evictedBytes / 1048576.0)
       << "}";
    return os.str();
}

std::string OfflineCacheManager::roomStateToJson(const RoomCacheState& state) const {
    std::ostringstream os;
    os << R"({"room_id":")" << state.roomId
       << R"(","cached_mb":)" << (state.cachedBytes / 1048576.0)
       << R"(,"messages":)" << state.cachedMessages
       << R"(,"media":)" << state.cachedMedia
       << R"(,"priority":)" << state.priority
       << R"(,"is_complete":)" << (state.isComplete ? "true" : "false")
       << "}";
    return os.str();
}

std::string OfflineCacheManager::pressureToJson(CachePressure pressure) const {
    std::ostringstream os;
    os << R"({"level":)" << static_cast<int>(pressure)
       << R"(,"label":")";
    switch (pressure) {
        case CachePressure::NONE: os << "plenty of space"; break;
        case CachePressure::LOW: os << "monitoring"; break;
        case CachePressure::MEDIUM: os << "starting eviction"; break;
        case CachePressure::HIGH: os << "aggressive eviction"; break;
        case CachePressure::CRITICAL: os << "emergency — < 10 MB free"; break;
    }
    os << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
