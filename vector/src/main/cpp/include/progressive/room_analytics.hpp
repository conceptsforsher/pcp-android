#ifndef PROGRESSIVE_ROOM_ANALYTICS_HPP
#define PROGRESSIVE_ROOM_ANALYTICS_HPP

#include <string>
#include <vector>
#include "progressive/well_known.hpp"
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Room User Analytics ----

struct UserStats {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    std::string serverName;        // extracted from @user:server
    int messageCount = 0;
    int64_t firstSeenMs = 0;       // join event timestamp
    int64_t lastSeenMs = 0;        // last message timestamp
    int64_t avgMessageLength = 0;   // avg chars per message
    bool isModerator = false;
};

struct JoinLeaveEvent {
    std::string eventId;
    std::string type;              // "join" or "leave"
    int64_t timestamp = 0;
};

struct RoomAnalytics {
    std::string roomId;
    int totalMessages = 0;
    int totalUsers = 0;
    std::vector<std::string> servers;           // unique servers
    std::vector<UserStats> topPosters;          // sorted by messageCount desc
    std::unordered_map<std::string, std::vector<JoinLeaveEvent>> userJoinHistory; // userId → join/leave events
};

// Compute room analytics from cached events.
RoomAnalytics computeRoomAnalytics(
    const std::vector<struct CacheEvent>& events
);

// Extract server name from MXID: @user:matrix.org → "matrix.org" (see well_known.hpp)

// Sort users by join date (oldest first).
void sortByJoinDate(std::vector<UserStats>& users);

// Filter users by server name.
std::vector<UserStats> filterByServer(const std::vector<UserStats>& users, const std::string& server);

// Export analytics as JSON.
std::string analyticsToJson(const RoomAnalytics& analytics);

// Compute average message length per user.
std::vector<UserStats> computeAvgMessageLengths(
    const std::vector<struct CacheEvent>& events
);

// ---- Cache Event (shared struct) ----

struct CacheEvent {
    std::string eventId;
    std::string roomId;
    std::string senderId;
    std::string senderName;
    std::string body;
    std::string eventType;   // "m.room.message", "m.room.member", etc.
    std::string stateKey;    // for membership events
    std::string membership;  // "join", "leave", "invite"
    int64_t timestamp = 0;
};

} // namespace progressive

#endif // PROGRESSIVE_ROOM_ANALYTICS_HPP
