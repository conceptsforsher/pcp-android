#include "progressive/cache_manager.hpp"
#include <sstream>
#include <ctime>
#include <algorithm>
#include <unordered_set>

namespace progressive {

void CacheManager::track(const std::string& eventId, const std::string& roomId,
                         const std::string& roomName, int64_t timestamp, int64_t sizeBytes,
                         const std::string& msgType, const std::string& body) {
    // Update if exists, otherwise add
    for (auto& e : entries_) {
        if (e.eventId == eventId) {
            e.roomName = roomName;
            e.timestamp = timestamp;
            e.sizeBytes = sizeBytes;
            e.msgType = msgType;
            e.body = body;
            return;
        }
    }
    entries_.push_back({eventId, roomId, roomName, body, timestamp, sizeBytes, msgType});
}

CacheStats CacheManager::getStats() const {
    CacheStats stats;
    stats.entryCount = static_cast<int>(entries_.size());

    std::unordered_set<std::string> rooms;
    int64_t oldest = 0, newest = 0;

    for (const auto& e : entries_) {
        stats.totalBytes += e.sizeBytes;
        rooms.insert(e.roomId);
        if (oldest == 0 || e.timestamp < oldest) oldest = e.timestamp;
        if (e.timestamp > newest) newest = e.timestamp;
    }

    stats.roomCount = static_cast<int>(rooms.size());

    if (oldest > 0) {
        time_t ts = oldest / 1000;
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&ts));
        stats.oldestEntryDate = buf;
    }
    if (newest > 0) {
        time_t ts = newest / 1000;
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&ts));
        stats.newestEntryDate = buf;
    }

    return stats;
}

std::vector<CacheEntry> CacheManager::getByRoom(const std::string& roomId) const {
    std::vector<CacheEntry> result;
    for (const auto& e : entries_) {
        if (e.roomId == roomId) result.push_back(e);
    }
    return result;
}

std::vector<CacheEntry> CacheManager::getOlderThan(int64_t beforeTs) const {
    std::vector<CacheEntry> result;
    for (const auto& e : entries_) {
        if (e.timestamp < beforeTs) result.push_back(e);
    }
    return result;
}

std::vector<CacheEntry> CacheManager::getByDateRange(int64_t fromTs, int64_t toTs) const {
    std::vector<CacheEntry> result;
    for (const auto& e : entries_) {
        if (e.timestamp >= fromTs && e.timestamp <= toTs) result.push_back(e);
    }
    return result;
}

std::vector<std::string> CacheManager::markForDeletion(const std::vector<std::string>& eventIds) {
    std::unordered_set<std::string> toDelete(eventIds.begin(), eventIds.end());
    std::vector<std::string> deleted;

    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const CacheEntry& e) {
            if (toDelete.count(e.eventId)) {
                deleted.push_back(e.eventId);
                return true;
            }
            return false;
        }
    ), entries_.end());

    return deleted;
}

void CacheManager::clear() {
    entries_.clear();
}

std::vector<std::string> CacheManager::getCachedRoomIds() const {
    std::unordered_set<std::string> seen;
    std::vector<std::string> result;
    for (const auto& e : entries_) {
        if (seen.insert(e.roomId).second) {
            result.push_back(e.roomId);
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

std::string CacheManager::statsToJson() const {
    auto stats = getStats();
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("entryCount": )" << stats.entryCount << ",";
    json << R"("roomCount": )" << stats.roomCount << ",";
    json << R"("totalBytes": )" << stats.totalBytes << ",";
    json << R"("oldestDate": ")" << esc(stats.oldestEntryDate) << R"(",)";
    json << R"("newestDate": ")" << esc(stats.newestEntryDate) << R"(")";
    json << "}";
    return json.str();
}

std::string CacheManager::entriesToJson(const std::vector<CacheEntry>& entries) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < entries.size(); ++i) {
        if (i > 0) json << ",";
        const auto& e = entries[i];
        json << R"({"eventId": ")" << esc(e.eventId) << R"(")";
        json << R"(,"roomId": ")" << esc(e.roomId) << R"(")";
        json << R"(,"roomName": ")" << esc(e.roomName) << R"(")";
        json << R"(,"body": ")" << esc(e.body) << R"(")";
        json << R"(,"timestamp": )" << e.timestamp;
        json << R"(,"sizeBytes": )" << e.sizeBytes << "}";
    }
    json << "]";
    return json.str();
}

} // namespace progressive
