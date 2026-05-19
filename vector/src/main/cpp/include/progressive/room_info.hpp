#ifndef PROGRESSIVE_ROOM_INFO_HPP
#define PROGRESSIVE_ROOM_INFO_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct RoomInfo {
    std::string roomId;
    std::string roomName;
    int64_t creationTs = 0;         // epoch ms of room creation
    std::string creationDate;        // ISO 8601
    int cachedEventCount = 0;
    int estimatedTotalEvents = 0;    // from sync token / server hints
    bool likelyFullHistory = false;  // cached count ≈ estimated total
};

// Compute whether the cached history is likely complete
// (cached >= 90% of estimated total, or estimated total is 0/unknown)
bool isLikelyFullHistory(int cachedCount, int estimatedTotal);

// Format a creation timestamp as human-readable date
std::string formatCreationDate(int64_t epochMs);

// Build room info JSON for UI display
std::string roomInfoToJson(const RoomInfo& info);

// Estimate total events from a sync token or room state
// If we have a prev_batch token, the room has more history
// Returns 0 if unknown
int estimateTotalEvents(int cachedCount, bool hasMoreHistory);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_INFO_HPP
