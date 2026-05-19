#include "progressive/eventcache.hpp"
#include <sstream>
#include <android/log.h>
#include <chrono>

#define LOG_TAG "EventCache"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace progressive {

void EventCache::put(const CachedEvent& event) {
    // Update reverse index: remove old relation if any
    auto it = events_.find(event.eventId);
    if (it != events_.end() && !it->second.sourceEventId.empty()) {
        auto& vec = relationIndex_[it->second.sourceEventId];
        vec.erase(std::remove(vec.begin(), vec.end(), event.eventId), vec.end());
    }

    // Store the event
    events_[event.eventId] = event;

    // Update reverse index
    if (!event.sourceEventId.empty()) {
        relationIndex_[event.sourceEventId].push_back(event.eventId);
    }

    // Track metadata for cache management
    int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    auto metaIt = metadata_.find(event.eventId);
    if (metaIt != metadata_.end()) {
        metaIt->second.lastAccessedAt = now;
        metaIt->second.accessCount++;
    } else {
        CacheEntryMetadata meta;
        meta.addedAt = now;
        meta.lastAccessedAt = now;
        meta.accessCount = 1;
        meta.size = sizeof(CachedEvent) + event.body.size() + event.formattedBody.size();
        metadata_[event.eventId] = meta;
    }
}

const CachedEvent* EventCache::get(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it != events_.end()) {
        hits_++;
        auto metaIt = metadata_.find(eventId);
        if (metaIt != metadata_.end()) {
            metaIt->second.lastAccessedAt =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            metaIt->second.accessCount++;
        }
        return &it->second;
    }
    misses_++;
    return nullptr;
}

std::vector<const CachedEvent*> EventCache::getRelations(const std::string& sourceEventId) const {
    std::vector<const CachedEvent*> results;
    auto idxIt = relationIndex_.find(sourceEventId);
    if (idxIt != relationIndex_.end()) {
        for (const auto& relatedId : idxIt->second) {
            auto eventIt = events_.find(relatedId);
            if (eventIt != events_.end()) {
                results.push_back(&eventIt->second);
            }
        }
    }
    return results;
}

void EventCache::clear() {
    events_.clear();
    relationIndex_.clear();
    metadata_.clear();
    hits_ = 0;
    misses_ = 0;
    evictions_ = 0;
}

std::string EventCache::getContextData(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it == events_.end()) {
        return R"({"cached": false})";
    }

    const auto& e = it->second;
    std::ostringstream json;

    json << "{";
    json << R"("cached": true,)";
    json << R"("eventId": ")" << e.eventId << R"(",)";
    json << R"("senderId": ")" << e.senderId << R"(",)";
    json << R"("senderName": ")" << e.senderName << R"(",)";
    json << R"("timestamp": ")" << e.timestamp << R"(",)";
    json << R"("body": ")" << e.body << R"(",)";
    json << R"("msgType": ")" << e.msgType << R"(",)";
    json << R"("eventType": ")" << e.eventType << R"(",)";
    json << R"("sentByMe": )" << (e.sentByMe ? "true" : "false") << ",";
    json << R"("hasFailed": )" << (e.hasFailed ? "true" : "false") << ",";
    json << R"("reactionCount": )" << e.reactionCount << ",";
    json << R"("hasBeenEdited": )" << (e.hasBeenEdited ? "true" : "false");

    // Add relation info if present
    if (!e.relationType.empty()) {
        json << R"(,"relationType": ")" << e.relationType << R"(")";
        json << R"(,"sourceEventId": ")" << e.sourceEventId << R"(")";
    }

    // Add reactions summary
    auto relations = getRelations(eventId);
    if (!relations.empty()) {
        json << R"(,"reactions": [)";
        for (size_t i = 0; i < relations.size(); ++i) {
            if (i > 0) json << ",";
            json << R"({"key": ")" << relations[i]->body
                 << R"(", "count": )" << relations[i]->reactionCount
                 << R"(", "addedByMe": )" << (relations[i]->sentByMe ? "true" : "false")
                 << "}";
        }
        json << "]";
    }

    json << "}";
    return json.str();
}

// ================================================================
// Extended Cache Management
// ================================================================

// Original Kotlin: configure()
void EventCache::configure(const EventCacheConfig& config) {
    config_ = config;
}

// Original Kotlin: getConfig()
const EventCacheConfig& EventCache::getConfig() const {
    return config_;
}

// Original Kotlin: getCacheStats()
EventCacheStats EventCache::getCacheStats() const {
    EventCacheStats stats;
    stats.hits = hits_;
    stats.misses = misses_;
    stats.evictions = evictions_;
    stats.size = events_.size();
    int64_t total = hits_ + misses_;
    stats.hitRate = total > 0 ? static_cast<double>(hits_) / static_cast<double>(total) : 0.0;
    return stats;
}

// Original Kotlin: evictByPolicy()
void EventCache::evictByPolicy() {
    if (events_.empty()) return;

    switch (config_.policy) {
        case EventCachePolicy::LRU: {
            std::string oldestKey;
            int64_t oldestTime = INT64_MAX;
            for (const auto& kv : events_) {
                auto mit = metadata_.find(kv.first);
                if (mit != metadata_.end() && mit->second.lastAccessedAt < oldestTime) {
                    oldestTime = mit->second.lastAccessedAt;
                    oldestKey = kv.first;
                }
            }
            if (!oldestKey.empty()) {
                events_.erase(oldestKey);
                metadata_.erase(oldestKey);
                for (auto& ri : relationIndex_) {
                    auto& vec = ri.second;
                    vec.erase(std::remove(vec.begin(), vec.end(), oldestKey), vec.end());
                }
                evictions_++;
            }
            break;
        }
        case EventCachePolicy::FIFO: {
            std::string oldestKey;
            int64_t oldestTime = INT64_MAX;
            for (const auto& kv : events_) {
                auto mit = metadata_.find(kv.first);
                if (mit != metadata_.end() && mit->second.addedAt < oldestTime) {
                    oldestTime = mit->second.addedAt;
                    oldestKey = kv.first;
                }
            }
            if (!oldestKey.empty()) {
                events_.erase(oldestKey);
                metadata_.erase(oldestKey);
                for (auto& ri : relationIndex_) {
                    auto& vec = ri.second;
                    vec.erase(std::remove(vec.begin(), vec.end(), oldestKey), vec.end());
                }
                evictions_++;
            }
            break;
        }
        case EventCachePolicy::TTL: {
            int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            std::vector<std::string> toRemove;
            for (const auto& kv : events_) {
                auto mit = metadata_.find(kv.first);
                if (mit != metadata_.end()) {
                    int64_t age = now - mit->second.addedAt;
                    if (age > config_.ttlMs) {
                        toRemove.push_back(kv.first);
                    }
                }
            }
            for (const auto& key : toRemove) {
                events_.erase(key);
                metadata_.erase(key);
                for (auto& ri : relationIndex_) {
                    auto& vec = ri.second;
                    vec.erase(std::remove(vec.begin(), vec.end(), key), vec.end());
                }
                evictions_++;
            }
            break;
        }
    }
}

// Original Kotlin: isEvictionNeeded()
bool EventCache::isEvictionNeeded() const {
    return events_.size() >= config_.maxSize;
}

// Original Kotlin: warmCache()
void EventCache::warmCache(const std::vector<CachedEvent>& events) {
    for (const auto& event : events) {
        put(event);
    }
    LOGD("EventCache warmed with %zu events", events.size());
}

// Original Kotlin: invalidateRoom()
void EventCache::invalidateRoom(const std::string& roomId) {
    (void)roomId;
    // Clear all events — room-level granularity requires roomId field on CachedEvent
    std::vector<std::string> toRemove;
    for (const auto& kv : events_) {
        toRemove.push_back(kv.first);
    }
    for (const auto& key : toRemove) {
        events_.erase(key);
        metadata_.erase(key);
    }
    for (auto& ri : relationIndex_) {
        ri.second.clear();
    }
    LOGD("EventCache invalidated room %s", roomId.c_str());
}

// Original Kotlin: invalidateSender()
void EventCache::invalidateSender(const std::string& senderId) {
    std::vector<std::string> toRemove;
    for (const auto& kv : events_) {
        if (kv.second.senderId == senderId) {
            toRemove.push_back(kv.first);
        }
    }
    for (const auto& key : toRemove) {
        events_.erase(key);
        metadata_.erase(key);
        for (auto& ri : relationIndex_) {
            auto& vec = ri.second;
            vec.erase(std::remove(vec.begin(), vec.end(), key), vec.end());
        }
    }
    LOGD("EventCache invalidated sender %s (%zu events removed)",
         senderId.c_str(), toRemove.size());
}

// Original Kotlin: getCacheHitRate()
double EventCache::getCacheHitRate() const {
    return getCacheStats().hitRate;
}

} // namespace progressive
