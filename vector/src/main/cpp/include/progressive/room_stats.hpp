#ifndef PROGRESSIVE_ROOM_STATS_HPP
#define PROGRESSIVE_ROOM_STATS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct RoomStats {
    std::string roomId;
    int totalEvents = 0;
    int textMessages = 0;
    int imageMessages = 0;
    int videoMessages = 0;
    int fileMessages = 0;
    int audioMessages = 0;
    int emoteMessages = 0;
    int reactionEvents = 0;
    int memberEvents = 0;
    int encryptedEvents = 0;
    int redactedEvents = 0;
    int64_t firstEventTs = 0;
    int64_t lastEventTs = 0;
    double messagesPerDay = 0.0;
    int uniqueSenders = 0;
    std::string busiestHour; // "14:00-15:00"
    int busiestHourCount = 0;
    int64_t totalBodyChars = 0;
    int avgMessageLength = 0;
};

// Compute room statistics from event data.
RoomStats computeRoomStats(
    const std::string& roomId,
    const std::vector<std::string>& eventTypes,
    const std::vector<std::string>& msgTypes,
    const std::vector<std::string>& senders,
    const std::vector<int64_t>& timestamps,
    const std::vector<bool>& encrypted,
    const std::vector<bool>& redacted,
    const std::vector<std::string>& bodies
);

// Compute messages per day from event timestamps.
double computeMessagesPerDay(int eventCount, int64_t firstTs, int64_t lastTs);

// Find the busiest hour of the day.
std::pair<int, int> findBusiestHour(const std::vector<int64_t>& timestamps);

// Format room stats as JSON.
std::string roomStatsToJson(const RoomStats& stats);

// Format room stats as human-readable text.
std::string roomStatsToText(const RoomStats& stats);

} // namespace progressive

#endif // PROGRESSIVE_ROOM_STATS_HPP
